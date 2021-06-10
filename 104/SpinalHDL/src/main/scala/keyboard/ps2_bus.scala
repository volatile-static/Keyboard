package keyboard

import spinal.core._
import spinal.lib._

class ps2_bus(clkDomain: ClockDomain, byteDelay: UInt) extends BlackBox {
  addGeneric("BYTE_DELAY", byteDelay)
  val clock_quarter, reset: Bool = in Bool
  val tx_failed: Bool = out Bool
  val tx: Stream[Bits] = slave Stream Bits(8 bits)
  val rx: Flow[Bits] = master Flow Bits(8 bits)
  val PS2: ps2_bundle = new ps2_bundle
  mapClockDomain(clkDomain, clock_quarter, reset)
}

case class ps2_bundle() extends Bundle {
  val CLK, DAT = inout(Analog(Bool))  // tri
}
