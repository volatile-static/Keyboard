import keyboard._
import org.apache.commons.io.FileUtils
import spinal.core._
import spinal.lib._
import spinal.lib.eda.altera._
import java.io.File

object GenerateTop extends App {
  new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(50 MHz),
    verbose = true
  ).generateSystemVerilog(new TopLevel).printPruned()
  val prj = new QuartusProject(
        "F:/intelFPGA_lite/20.1/quartus/bin64/",
        "../")
//  prj.compile()
//  prj.program()
}

object GenerateIP extends App {
  val cfg = new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(75 MHz),
    mode = SystemVerilog,
    globalPrefix = "IP_",
    targetDirectory = "../Qsys/Hardware/IP/"
  )
  def gen[T <: Component](m : SpinalReport[T]): Unit = {
//    QSysify(m.toplevel)
//    FileUtils.copyFileToDirectory(
//      new File(m.toplevelName + "_hw.tcl"),
//      new File("../Qsys/Hardware/IP/")
//    )
//    FileUtils.deleteQuietly(new File(m.toplevelName + "_hw.tcl"))
  }
  gen(cfg.generate(new KeyMM))
  gen(cfg.generate(new LedMatrix))
  gen(cfg.generate(new PS2Tx))
  gen(cfg.generate(new PS2Rx))
}

object Test extends App {
  case class Top() extends Component {

    val a: Bits = in Bits(103 bits)
//    val b: Vec[Flow[Bool]] = Vec(master(Flow(Bool())), 103)
//    val d: Debounce = Debounce(200 us, 1 ms)
//    a <> d.key_down
//    b <> d.keyStatus
//    a.addAttribute("altera_attribute", "-name VIRTUAL_PIN ON")
//    b.addAttribute("altera_attribute", "-name VIRTUAL_PIN ON")


    val o: Bool = out Bool()
    switch(a(0)) {
      is(True) {
        o := a(1)
      }
    }
  }

  new SpinalConfig(
    targetDirectory = "../tmp/",
    defaultClockDomainFrequency = FixedFrequency(50 MHz)
  ).generateVerilog(Top())

//  println(QuartusFlow(
//    quartusPath="E:\\DEVELOPMENT\\TOOLS\\Quartus\\quartus\\bin64",
//    workspacePath="../tmp/prj/",
//    toplevelPath="../tmp/Top.v",
//    family="Cyclone IV E",
//    device="EP4CE6E22C8",
//    frequencyTarget = 50 MHz
//  ))
}
