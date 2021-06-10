package keyboard

import spinal.core._
import spinal.lib._

case class KeyMatrix(scanPeriod: TimeNumber, bounceTime: TimeNumber) extends Component {
  val enabled: Bool = in Bool
  val COL: Bits = in Bits(21 bits)
  val ROW: Bits = out Bits(6 bits)
  val scanIdx: Flow[Bits] = master Flow Bits(8 bits)
  val keyStatus: Vec[Flow[Bool]] = Vec(Flow(Bool), 103)
  val keyBits: Bits = out(keyStatus.asBits)

  val clkScan: Timeout = Timeout(scanPeriod.toHertz * 6)
  when(clkScan) (clkScan.clear)

  val scanK: scan_key = scan_key(
    ClockDomain(
      clock = clkScan,
      reset = ClockDomain.current.reset
    )
  )
  scanK.COL <> COL
  scanK.ROW <> ROW
  scanK.enabled := enabled

  val debounce: Debounce = Debounce(scanPeriod, bounceTime)
  debounce.key_down <> scanK.key_down
  debounce.keyStatus <> keyStatus

  new SlowArea((scanPeriod / 103).toHertz, true) {
    val cnt: Counter = CounterFreeRun(103)
    scanIdx.valid := keyStatus(cnt).valid
    scanIdx.payload := keyStatus(cnt).payload ## cnt
  }
}
