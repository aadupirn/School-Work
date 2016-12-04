import scala.io.Source
import org.apache.spark.rdd._
import scala.collection.mutable.Buffer
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.mllib.linalg.Matrices
import org.apache.spark.mllib.linalg.distributed._
import org.apache.spark.mllib.random._
import breeze.linalg._
import java.io._

object Assign2 {

    def factorize(mat: CoordinateMatrix,
                             W: DenseMatrix[Double],
                             H: DenseMatrix[Double],
                             k: Int,
                             alpha: Double,
                             beta: Double,
                             steps: Int,
                             minError: Double): DenseMatrix[Double] =
    {
        var step = 0
        val ki = 0
        var loop = true

        while (step < steps && loop)
        {
            val sample = mat.entries.takeSample(true, 1, System.nanoTime)(0)
            val i = sample.i.toInt
            val j = sample.j.toInt
            val value = sample.value

            val wi = W(i, ::)
            val hj = H(::, j)
            val eij = value - (wi * hj)

            for (ki <- 0 to k - 1)
            {
                W.update(i, ki, W(i,ki) + alpha * (2 * eij * H(ki, j) - beta * W(i, ki)))
                H.update(ki, j, H(ki,j) + alpha * (2 * eij * W(i, ki) - beta * H(ki, j)))
            }

            if (step % 10 == 0)
            {
                val newMat = W * H
                var e = 0.0
                mat.entries.foreach
                {
                    MatrixEntry =>
                    val i = MatrixEntry.i.toInt
                    val j = MatrixEntry.j.toInt
                    val value = MatrixEntry.value
                    e = e + ((value - newMat(i, j)) * (value - newMat(i, j)))
                    for (ki <- 0 to k - 1) {
                        e = e + (beta/2) * (W(i,ki)*W(i,ki) + H(ki,j)*H(ki,j))
                    }
                }
                if (e < minError) {
                // terminate loop after no change
                loop = false
                }
                step = step + 1
            }
        }
        return W * H
    }

    def RowToDense(mat:RowMatrix):DenseMatrix[Double] =
    {
        val m = mat.numRows()
        val n = mat.numCols()
        val result = DenseMatrix.zeros[Double](m.toInt,n.toInt)
        var i = 0

        mat.rows.collect().foreach{Vector =>
        Vector.foreachActive { case(index,value) =>
        result(i,index) = value
        }
        i+=1
        }
        result

    }

    def toDenseMatrix(mat:IndexedRowMatrix):DenseMatrix[Double] =
    {
        val numRows = mat.numRows().toInt
        val numCols = mat.numCols().toInt
        val resultMatrix = DenseMatrix.zeros[Double](numRows,numCols)
        var i = 0
        mat.rows.collect().sortBy(_.index).foreach
        {
            IndexedRow => IndexedRow.vector.foreachActive
            {
                case(index,value) => resultMatrix(i,index) = value
            }
            i = i+1
        }
        resultMatrix
    }


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
        )

        val numCols = mat.numCols.toInt
        val numRows = mat.numRows.toInt

        val missingMat = sc.textFile(missingfile).map(x => x.split(",")).map(x => (x(0).toInt, x(1).toInt, 0))

        val outputCount = missingMat.count.toInt

        val k = 7
        val i = 0
        val alpha = 0.0002
        val beta = 0.02
        val steps = 50000
        val minError = 0.0001

        //val svd =  mat.toIndexedRowMatrix.computeSVD(k, computeU = true)
        //val W = toDenseMatrix(svd.U.multiply(Matrices.diag(svd.s)))
        //val H = new DenseMatrix(k, numCols, svd.V.transpose.toArray)
        val WRowM = new RowMatrix(RandomRDDs.uniformVectorRDD(sc, numRows, k)) //uniform distribution from 0-1
        val W = RowToDense(WRowM)
        val HRowM = new RowMatrix(RandomRDDs.uniformVectorRDD(sc, k, numCols))
        val H = RowToDense(HRowM)

        val resultMat = factorize(mat, W, H, k, alpha, beta, steps, minError)

        val missingMatCollected = missingMat.collect

        //output
        for(i <- 0 to (outputCount-1))
        {
            val indI = missingMatCollected(i)._1
            val indJ = missingMatCollected(i)._2
            output.write(indI+","+indJ+","+resultMat(indI, indJ)+"\n")
        }

        //missingMat.foreach(
        //    a => output.write(a._1+","+a._2+","+newMat(a._1)(a._2)+"\n")
        //)
        //output.write(x._1+","+x._2+","+x._3+"\n") //need to write out values to missing coordinates

        output.close()
        System.exit(0)
    }
}
