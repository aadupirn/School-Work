object Part2 {
  def main(args: Array[String]): Unit = {
    val panelfile = "integrated_call_samples_v3.20130502.ALL.panel"
    import scala.io.Source
    val biggroups = Source.fromFile(panelfile).getLines().drop(1).toList.map(_.split("\t")(1)).groupBy(identity).filter(_._2.size>90)
    val people = Source.fromFile(panelfile).getLines().drop(1).toList.map(_.split("\t")).filter(x => biggroups.contains(x(1))).toSet

    println("Populations with more than 90 individuals: "+biggroups.size)
    println("Individuals from these populations: "+people.size)
  }
}
