package keyboard

import spinal.core._

case class scan_key(clkDomain: ClockDomain) extends BlackBox {
  val clock, reset, enabled: Bool = in Bool
  val COL: Bits = in Bits(21 bits)
  val ROW: Bits = out Bits(6 bits)
  val key_down: Bits = out Bits(103 bits)
  mapClockDomain(clkDomain, clock, reset)
}
