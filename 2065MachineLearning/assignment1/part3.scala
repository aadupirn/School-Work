import org.bdgenomics.adam.rdd.ADAMContext._
import collection.JavaConverters._

def convertAlleles(x: java.util.List[org.bdgenomics.formats.avro.GenotypeAllele]) = 
{
	x.asScala.map(_.toString) 
}

val data = sc.loadGenotypes("/home/aadu/Repos/School-Work/2065/assignment1/small.adam")

val peopleCount = data.rdd.map(r => r.sampleId).distinct.count

val peopleToVariants = data.rdd.map(r => r.sampleId -> (r.start, r.end, convertAlleles(r.alleles))).distinct

peopleToVariants.take(3)


