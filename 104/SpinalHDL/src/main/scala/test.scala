package mylib

import spinal.core._
import spinal.core.sim._
import spinal.lib._
import keyboard._
import spinal.lib.eda.altera._

object Test extends App {
  case class SingleKey(cntInit: BigInt) extends Component {
    val raw: Bool = in Bool()
    val filtered: Flow[Bool] = master Flow Bool

    val lat: Bool = Delay(!raw, 2)
    val cnt: UInt = RegInit(cntInit)
    when(cnt =/= 0) (cnt := cnt - 1)
    when(raw.edge) { cnt := U(cntInit) }
    filtered.valid := RegNext(cnt === 1)
    filtered.payload := RegNextWhen(lat, cnt === 1)
    println(s"latency ${LatencyAnalysis(raw, filtered.payload)}\n")
  }
    SimConfig.withWave.doSim(SingleKey(2)) { dut =>
      dut.clockDomain.forkStimulus(10)
      val signalList = Array(0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1)
      for (i <- signalList) {
        dut.clockDomain.waitSampling()
        dut.raw #= i.toBoolean
      }
    }

  case class Tmp() extends Component {
    val io = new Bundle {
      val sel = in UInt(7 bits)
    }
  }

  case class Top() extends Component {
    val a: Bits = in Bits(103 bits)
    val io = new Bundle {
//      val miso: Stream[Bits] = master Stream Bits(8 bits)
//      val mosi: Stream[Bits] = slave Stream Bits(8 bits)
    }
  }

//  new SpinalConfig(
//    targetDirectory = "../tmp/",
//    defaultClockDomainFrequency = FixedFrequency(50 MHz)
//  ).generateVerilog(HID())

//    println(QuartusFlow(
//      quartusPath="/home/intelFPGA_lite/21.1/quartus/bin/",
//      workspacePath="../tmp/prj/",
//      toplevelPath="../tmp/HID.v",
//      family="Cyclone IV E",
//      device="EP4CE10E22C8",
//      frequencyTarget = 50 MHz
//    ))

//  SimConfig.withWave.doSim(new Tmp) { dut =>
//    dut.clockDomain.forkStimulus(10)
//    dut.clockDomain.waitSampling()
//    dut.io.sel #= 2L
//  }
}
