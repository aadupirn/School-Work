import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.rdd._
import org.bdgenomics.adam.rdd.ADAMContext._
import collection.JavaConverters._
import scala.io.Source
import scala.util.Random
import math._
import org.apache.spark.sql.SQLContext
import org.apache.spark.mllib.linalg._
import org.apache.spark.ml.feature.VectorAssembler

object Assign1 {
    def main(args: Array[String])
    {
        val conf - new SparkConf().setAppName("Assign2")
        val sc = new SparkContext

        val csv = "small.txt"
        //val csv = args(0)

        val mat = new CoordinateMatrix(
            sc.textFile(csv)
            .map(x => x.split(","))
            .map(x => MatrixEntry(x(0).toLong, x(1).toLong, x(2).toFloat))
        )

        val colCount = mat.numCols.toInt
        val rowsCount = mat.numRows.toInt
        val k = 3

        








        System.exit(0)
    }
}
