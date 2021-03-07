package keyboard

import spinal.core._
import spinal.lib._

class ps2_bus(byteDelay: UInt) extends BlackBox {
  addGeneric("BYTE_DELAY", byteDelay)
  val clock_quarter, reset: Bool = in Bool
  val tx_failed: Bool = out Bool
  val tx: Stream[Bits] = slave Stream Bits(8 bits)
  val rx: Flow[Bits] = master Flow Bits(8 bits)
  val PS2_CLK, PS2_DAT = inout(Analog(Bool)) // tri
}
