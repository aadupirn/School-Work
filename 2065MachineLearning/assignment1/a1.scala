import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.rdd._
import org.bdgenomics.adam.rdd.ADAMContext._
import collection.JavaConverters._
import scala.io.Source
import scala.util.Random
import math._

object Assign1 {
    def main(args: Array[String])
    {
        val panelfile = args(0)
        val adamfile = args(1)
        //val panelfile = "integrated_call_samples_v3.20130502.ALL.panel"
        //val adamfile = "small.adam"

        val conf = new SparkConf().setAppName("Assign1")
        val sc = new SparkContext(conf)

        val biggroups = Source.fromFile(panelfile).getLines().drop(1).toList.map(_.split("\t")(1)).groupBy(identity).filter(_._2.size>90)
        val people = Source.fromFile(panelfile).getLines().drop(1).toList.map(_.split("\t")).filter(x => biggroups.contains(x(1))).map(r => r(0)).toSet

        println("Populations with more than 90 individuals: " + biggroups.size)
        println("Individuals from these populations: " + people.size)

    	def convertAlleles(x: java.util.List[org.bdgenomics.formats.avro.GenotypeAllele]) ={
            x.asScala.map(r => if(r.toString == "Ref") 0 else 1).reduce(_+_).toInt}
        val data = sc.loadGenotypes(adamfile)

        val badCount = data.rdd.map(r => r.contigName -> (r.start, r.end)).distinct.count
        val peopleCount = data.rdd.map(r => r.sampleId).distinct.count.persist()
        val goodVariants = data.rdd.map(r => (r.contigName, r.start, r.end) -> 1).reduceByKey(_ + _).filter(_._2 == peopleCount).map(r => (r._1._1, r._1._2, r._1._3)).collect.toSet.persist()

        println("Total variants: " + badCount)
        println("Variants with right number of samples: " + goodVariants.size)

        val goodData = data.rdd.filter(r => people contains r.sampleId).map(r => ((r.start.toLong, r.end.toLong), r.sampleId, convertAlleles(r.alleles)))

        goodData.persist()

        var varsizes = goodData.mapPartitions( iter => {
            var res = collection.mutable.HashMap.empty[(Long, Long), Array[Int]]
            iter.foreach(rec => {
                var cur = res.getOrElseUpdate(rec._1, Array.fill(4)(0))
                cur(3) += 1
            })
            res.iterator
        }).reduceByKey((accum,n) => ({accum(0)+=n(0); accum(1)+=n(1); accum(2)+=n(2); accum(3) += n(3); accum})).map(r => (r._1, (r._2(3),(r._2(0)+r._2(1)+r._2(2))))).persist()

        val rddGoodData = varsizes.filter(r => r._2._1 == people.size).persist()

        val varindices = rddGoodData.filter(r => r._2._2 != 1).map(r => r._1).zipWithIndex().collectAsMap

        val vi = sc.broadcast(varindices)

        val preVectorData = goodData.mapPartitions( iter => {
            var res = collection.mutable.ArrayBuffer.empty[(String, (Int, Int))]
            val vari = vi.value
            iter.foreach(rec => {
                if(vari contains rec._1 ) {
                    val aval = rec._3
                    if(aval > 0) {
                        val index = vari( rec._1).toInt
                        res += ((rec._2, (index, aval) ))
                    }
                }
            })
            res.iterator
        })

        def makeVectors(x: Iterable[(Int, Int)]): Array[Float] = {
             var arr = Array.fill(goodVariants.size.toInt)(0.0f)
             x.foreach( {case (index, value) => { arr(index) = value } })
             return arr
        }

        val vectors = preVectorData.groupByKey().map(r => (r._1, makeVectors(r._2))).persist()

        //original way of calculating vectors.
        //val vectors = goodData.map(r => r._4 -> Array(r._5.count(_ =="Alt").toFloat)).reduceByKey(_++_)
        //


        val K = 21
        val MAX = 100
        var iterations = 0;
        val vectorCount = vectors.collect.size
        val variantCount = goodVariants.size

        def distance(xs: Array[Float], ys: Array[Float]) = {
            sqrt((xs zip ys).map { case (x,y) => pow(y - x, 2) }.sum)
        }

        def findNearestCluster(testVectors: Array[Array[Float]], clusters: Array[Array[Float]]): Array[Int] = {
            testVectors.map {
                testInstance => clusters.zipWithIndex.map {
                    case (trainInstance, c) => c -> distance(testInstance, trainInstance)
                }.minBy(_._2)._1
            }
        }

        def addVectors  (s: Array[Float], v: Array[Float] ): Array[Float]  = {
            for(i <- 0 until s.length){
                s(i) += v(i)
            }
            return s
        }

        var centers = vectors.takeSample(false, K, System.nanoTime.toInt).map(r=> r._2)

        var converged = false
        val vectorArray = vectors.map(r=>r._2).collect
        var clusterAssignments = findNearestCluster(vectorArray, centers)
        var previousAssignments = clusterAssignments
        var clusteredVectors = new Array[Array[Float]](K)
        var zeroArray = new Array[Float](variantCount)
        var i = 0
        for(i <- 0 to (variantCount-1))
        {
            zeroArray(i) = 0
        }

        while(!converged && iterations < MAX)
        {
            for(i <- 0 to (K-1))
            {
                var j = 0
                var clusterSize = 0.toFloat
                var l = 0
                var zeroArray = new Array[Float](variantCount)
                var i = 0
                for(l <- 0 to (variantCount-1))
                {
                    zeroArray(i) = 0
                }
                for(l <- 0 to (K-1))
                {
                    clusteredVectors(i) = zeroArray
                }
                for(j <- 0 to (vectorCount-1))
                {
                    if(clusterAssignments(j) == i)
                    {
                        if(clusterSize == 0)
                        {
                            clusteredVectors(i) = addVectors(zeroArray, vectorArray(j))
                        }
                        else
                        {
                            clusteredVectors(i) = addVectors(clusteredVectors(i), vectorArray(j))
                        }
                        clusterSize = clusterSize + 1
                    }
                }

                for(j <- 0 to (variantCount-1))                {

                    clusteredVectors(i)(j) = clusteredVectors(i)(j)/clusterSize
                }
                centers(i) = clusteredVectors(i)


            }

            previousAssignments = clusterAssignments
            clusterAssignments = findNearestCluster(vectorArray, centers)
            converged = true
            for(i <- 0 to (vectorCount-1))
            {
                if(previousAssignments(i) != clusterAssignments(i))
                {
                    converged = false
                }
            }
            iterations = iterations + 1
        }

        println("Clusters:")
        var fullVectorArray = vectors.collect
        for(i <- 0 to (K-1))
        {
            var j = 0
            for(j <- 0 to (vectorCount-1))
            {
                if(clusterAssignments(j) == i)
                {
                    print(fullVectorArray(j)._1 + " ")
                }
            }
            print("\n")
        }








        System.exit(0)
    }
}
