import keyboard._
import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._
import spinal.lib.bus.regif._
import spinal.lib.com.uart._
import spinal.lib.eda.altera._

object GenerateTop extends App {
  new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(50 MHz)
  ).generateSystemVerilog(new TopLevel)
  val prj = new QuartusProject(
        "F:/intelFPGA_lite/20.1/quartus/bin64/",
        "../")
  prj.compile()
  prj.program()
}

object GenerateIP extends App {
  new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(100 MHz)
  ).generateSystemVerilog(new LedMatrix)
}

object Test extends App {
  case class Top() extends Component {
    val a: Stream[Bits] =  Stream (Bits(8 bits))
    val b: Stream[Bits] =  Stream (Bits(8 bits))

    val t: Stream[Stream[Bits]] = a ~ b

  }

  new SpinalConfig(
    targetDirectory = "../tmp/",
    defaultClockDomainFrequency = FixedFrequency(50 MHz)
  ).generateSystemVerilog(Top())

//  println(QuartusFlow(
//    quartusPath="F:/intelFPGA_lite/20.1/quartus/bin64/",
//    workspacePath="../tmp/prj/",
//    toplevelPath="../tmp/Top.sv",
//    family="Cyclone IV",
//    device="EP4CE6E22C8",
//    frequencyTarget = 50 MHz
//  ))
}

//    val a: Bits = in Bits(20 bits)
//    val b = Vec(master(Flow(Bool())), 20)
//    val d: Debounce = Debounce(200 us, 1 ms)
//
//    d.key_down := B(0)
//    a <> d.key_down(0, 20 bits)
//    (0 to 19).foreach(i => b(i) := d.keyStatus(i))