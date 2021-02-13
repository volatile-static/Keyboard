package keyboard

import spinal.core._
import spinal.lib.eda.altera._

class TopLevel() extends Component {
  val io = new Bundle() {
    noIoPrefix()
    val KEY_Fn, CLK_50 = in Bool
    val COL: Bits = in Bits(21 bits)
    val LED_A, ROW = out Bits(6 bits)
    val LED_K: Bits = out Bits(35 bits)
    val TXD, LED_R6 = out Bool
    val PS2_CLK, PS2_DAT = inout(Analog(Bool))  // tri
    val flash: Bundle = new Bundle {
      val dclk, sce, sdo = out Bool
      val data0: Bool = in Bool
    }
  }

  val rstSrc: reset_controller = reset_controller()
  rstSrc.clock := io.CLK_50
  rstSrc.KEY_Fn := io.KEY_Fn
  val reset: Bool = Bool
  reset := rstSrc.reset

  io.LED_R6 := False

  val clk50Area: ClockingArea = new ClockingArea(ClockDomain(
    clock = io.CLK_50,
    reset = reset,
    frequency = FixedFrequency(50 MHz)
  )) {
    val keyMatrix: KeyMatrix = KeyMatrix(200 us, 500 us)
    io.ROW <> keyMatrix.ROW
    io.COL <> keyMatrix.COL
    keyMatrix.enabled := True

    val core: kbd104 = kbd104()
    core.uart.rxd := FlowToUart(keyMatrix.scanIdx)
    core.io_row <> io.LED_A
    core.io_col <> io.LED_K
    core.flash <> io.flash
//    val ps2: PS2 = PS2()
//    ps2.PS2_CLK <> io.PS2_CLK
//    ps2.PS2_DAT <> io.PS2_DAT
//    ps2.scanIdx <> keyMatrix.scanIdx

    val hid: HID = HID()
    hid.TXD <> io.TXD
    hid.scanIdx <> keyMatrix.scanIdx
    List(74, 85, 90, 91, 92, 94, 96)
      .foreach(i => hid.keyStatus(i) <> keyMatrix.keyStatus(i))
  }
  //    println(s"latency ${LatencyAnalysis(a,c)}")
}


object GenerateTop {
  def main(args: Array[String]) {
    new SpinalConfig(
      defaultClockDomainFrequency = FixedFrequency(50 MHz)
    ).generateSystemVerilog(new TopLevel)
//    spinal.lib.eda.altera
//    val prj = new QuartusProject(
//      "F:/intelFPGA_lite/20.1/quartus/bin64/",
//      "../")
//    prj.compile()
//    prj.program()
  }
}

object GenerateIP {
  def main(args: Array[String]): Unit = {
    new SpinalConfig(
      defaultClockDomainFrequency = FixedFrequency(80 MHz)
    ).generateSystemVerilog(new LedMatrix)

//    val ip = SpinalVerilog(LedPlayground()).toplevel
//    ip.bus.addTag(ClockDomainTag(ip.clockDomain))
//    QSysify(ip)
  }
}
