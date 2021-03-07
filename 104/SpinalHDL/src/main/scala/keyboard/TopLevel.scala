package keyboard

import spinal.core._
import spinal.lib.ResetCtrl

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

  val pll100: pll = pll()
  pll100.inclk0 <> io.CLK_50

  val rstCtrl: ResetController = ResetController()
  rstCtrl.io.clock <> io.CLK_50
  rstCtrl.io.externalReset <> !io.KEY_Fn

  val globalClockDomain = new ClockDomain(
    clock = io.CLK_50,
    reset = False.allowOverride,
    frequency = FixedFrequency(50 MHz)
  )
  ResetCtrl.asyncAssertSyncDeassertDrive(
    input = rstCtrl.io.globalReset || !pll100.locked,
    clockDomain = globalClockDomain,
    outputPolarity = HIGH
  )

  val core: kbd104 = kbd104()
  core.clk_clk <> pll100.c0
  core.reset_reset_n <> ~globalClockDomain.reset
  core.io_row <> io.LED_A
  core.io_col <> io.LED_K
  core.flash <> io.flash

  io.LED_R6 <> globalClockDomain.reset

  val clk50Area: ClockingArea = new ClockingArea(globalClockDomain) {
    val keyMatrix: KeyMatrix = KeyMatrix(500 us, 1 ms)
    io.ROW <> keyMatrix.ROW
    io.COL <> keyMatrix.COL
    keyMatrix.enabled := True
    core.uart.rxd := FlowToUart(keyMatrix.scanIdx)

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
