package keyboard

import spinal.core._
import spinal.lib._

class TopLevel() extends Component {
  val io = new Bundle() {
    noIoPrefix()
    val KEY_Fn, CLK_50 = in Bool
    val COL: Bits = in Bits(21 bits)
    val LED_A, ROW = out Bits(6 bits)
    val LED_K: Bits = out Bits(35 bits)
    val TXD, LED_R6 = out Bool
    val PS2 = new ps2_bundle
  }
  val rstCtrl: ResetController = new ResetController
  rstCtrl.io.clock <> io.CLK_50
  rstCtrl.io.externalReset <> !io.KEY_Fn

  val globalClockDomain = new ClockDomain(
    clock = io.CLK_50,
    reset = False.allowOverride,
    frequency = FixedFrequency(50 MHz)
  )
  val core: TPK = TPK(globalClockDomain)
  ResetCtrl.asyncAssertSyncDeassertDrive(
    input = rstCtrl.io.globalReset || !core.locked_export,
    clockDomain = globalClockDomain,
    outputPolarity = HIGH
  )
  core.led_row_export <> io.LED_A
  core.led_col_export <> io.LED_K
  io.LED_R6 := True

  val clk50Area: ClockingArea = new ClockingArea(globalClockDomain) {
    val keyMatrix: KeyMatrix = KeyMatrix(200 us, 5 ms)
    io.ROW <> keyMatrix.ROW
    io.COL <> keyMatrix.COL
    keyMatrix.enabled := True
    core.scan_idx <> keyMatrix.scanIdx

    val ps2: PS2Device = PS2Device()
    ps2.PS2 <> io.PS2
    ps2.sendToHost << core.send_to_host
    ps2.receiveFromHost.toStream >> core.receive_from_host

    val hid: HID = HID()
    hid.TXD <> io.TXD
    hid.scanIdx <> keyMatrix.scanIdx
    hid.keyBits <> keyMatrix.keyBits
  }
  //    println(s"latency ${LatencyAnalysis(a,c)}")
}
