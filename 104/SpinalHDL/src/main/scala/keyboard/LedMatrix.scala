package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._

case class LedMatrix(scanPeriod: TimeNumber = 50 us) extends Component {
  val io = new Bundle {
    val col: Bits = out Bits(35 bits)
    val row: Bits = out Bits(6 bits)
  }
  val bus: AvalonMM = slave(AvalonMM(
    AvalonMMConfig.fixed(
      addressWidth = 7,
      dataWidth = 32,
      readLatency = 0
    ).copy(useWaitRequestn = false).getWriteOnlyConfig
  ))
  bus addTag ClockDomainTag(ClockDomain.current)

  def busReg(address: Int): UInt =
    RegNextWhen(bus.writeData.asUInt, bus.write && bus.address === address, U(0))
  val regs = new Bundle {
    val ice: Vec[UInt] = Vec(UInt(8 bits), 73)
    val fn: UInt = UInt(16 bits)
    val locks: Vec[UInt] = Vec(UInt(8 bits), 3)
    val rgb: Vec[UInt] = Vec(UInt(24 bits), 30)
// TODO: 4*
    (0 until 73).foreach(i => ice(i) := busReg(i).resized)
    fn := busReg(73).resized
    (0 until 30).foreach(i => rgb(i) := busReg(74 + i).resized)
    locks := Vec(
      busReg(104).resized,
      busReg(105).resized,
      busReg(106).resized
    )
  }
  val ledSta: Bits = Bits(168 bits)

  val pwm: SlowArea = new SlowArea(scanPeriod.toHertz, true) {
    val pwmCnt: Counter = CounterFreeRun(256)
    ledSta(0 until 73) := regs.ice.map(pwmCnt > _).asBits
    ledSta(73 to 74) := (pwmCnt > regs.fn(0 to 7)) ## (pwmCnt > regs.fn(8 to 15))
    for (i <- 0 until 30)
      ledSta(75 + i*3, 3 bits) := Cat(
        pwmCnt > regs.rgb(i)(0 to 7),
        pwmCnt > regs.rgb(i)(8 to 15),
        pwmCnt > regs.rgb(i)(16 to 23)
      )
    ledSta(165, 3 bits) := regs.locks.map(pwmCnt > _).asBits
  }
  val scanner: SlowArea = new SlowArea(scanPeriod.toHertz * 6, true) {
    val rowCnt: Counter = CounterFreeRun(6)
    io.row := ~(B(1, 6 bits) |<< rowCnt)
    io.col := ~B(0, 35 bits)
    switch(rowCnt.value) {
      is(0) {
        io.col(0) := ledSta(0)
        io.col(2 to 13) := ledSta(1 to 12)
        io.col(14, 9 bits) := ledSta(75, 9 bits)
        io.col(28) := ledSta(165)
        io.col(31) := ledSta(166)
        io.col(34) := ledSta(167)
      }
      is(1) {
        io.col(0, 14 bits) := ledSta(13, 14 bits)
        io.col(14, 21 bits) := ledSta(84, 21 bits)
      }
      is(2) {
        io.col(0, 14 bits) := ledSta(27, 14 bits)
        io.col(14, 21 bits) := ledSta(105, 21 bits)
      }
      is(3) {
        io.col(0, 12 bits) := ledSta(41, 12 bits)
        io.col(13) := ledSta(53)
        io.col(23, 9 bits) := ledSta(126, 9 bits)
      }
      is(4) {
        io.col(0, 11 bits) := ledSta(54, 11 bits)
        io.col(13) := ledSta(65)
        io.col(17 to 19) := ledSta(135 to 137)
        io.col(23, 9 bits) := ledSta(138, 9 bits)
      }
      is(5) {
        io.col(0 to 2) := ledSta(66 to 68)
        io.col(5) := ledSta(69)
        io.col(9 to 10) := ledSta(70 to 71)
        io.col(11 to 12) := ledSta(73 to 74)  // Fn
        io.col(13, 13 bits) := ledSta(147, 12 bits) ## ledSta(72)
        io.col(29, 6 bits) := ledSta(159, 6 bits)
      }
    }
  }
}
