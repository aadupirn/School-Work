import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import scala.io.Source
import org.apache.spark.rdd._
import org.apache.spark.mllib.linalg._
import org.apache.spark.mllib.linalg.distributed._
import java.io._

object Assign2 {

    def main(args: Array[String])
    {
        val conf = new SparkConf().setAppName("Assign2")
        val sc = new SparkContext(conf)
        val datafile = args(0)
        val missingfile = args(1)
        val outfile = args(2)

        //val datafile = "small.csv"
        //val missingfile = "small_missing.csv"
        //val outfile = "out.txt"

        val ofile = new File(outfile)
        val output = new BufferedWriter(new FileWriter(ofile))

        //distributed matrix factorization
        val mat = new CoordinateMatrix(
            sc.textFile(datafile)
            .map(x => x.split(","))
            .map(x => MatrixEntry(x(0).toLong, x(1).toLong, x(2).toFloat))
        ).toRowMatrix

        val missingMat = sc.textFile(missingfile).map(x => x.split(",")).map(x => (x(0).toInt, x(1).toInt)).collect

        val outputCount = missingMat.size

        val k = 4
        var i = 0

        val svd =  mat.computeSVD(k, computeU = true)

        val newMat = svd.U.multiply(Matrices.diag(svd.s)).multiply(svd.V.transpose).rows.collect

        for(i <- 0 to (outputCount-1))
        {
            output.write(missingMat(i)._1+","+missingMat(i)._2+","+newMat(missingMat(i)._1)(missingMat(i)._2)+"\n")
        }

        //missingMat.foreach(a => output.write(a._1+","+a._2+","+newMat(a._1)(a._2)+"\n"))


        //output.write(x._1+","+x._2+","+x._3+"\n") //need to write out values to missing coordinates

        output.close()
        System.exit(0)
    }
}
