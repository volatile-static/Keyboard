import keyboard._
import spinal.core._
import spinal.lib._
import spinal.lib.eda.altera._
import spinal.lib.fsm._

import java.io.File
import java.nio.file._

object GenerateTop extends App {
  new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(50 MHz),
    targetDirectory = "/mnt/e/DEVELOPMENT/PROJECTS/TPK/SpinalHDL"
  ).generateSystemVerilog(new TopLevel)
  val prj = new QuartusProject(
        "/home/intelFPGA_lite/21.1/quartus/bin/",
        "../Quartus")
//  prj.compile()
//  prj.program()
}

object GenerateIP extends App {
  val ipPath = "/mnt/e/DEVELOPMENT/PROJECTS/TPK/Qsys/Hardware/IP/"
  val cfg = new SpinalConfig(
    defaultClockDomainFrequency = FixedFrequency(75 MHz),
    mode = SystemVerilog,
    globalPrefix = "IP_",
    targetDirectory = ipPath
  )
  val cfg1 = cfg.copy(globalPrefix = "PS2_")
  def gen[T <: Component](m : SpinalReport[T]): Unit = QSysify(m.toplevel, ipPath)

  gen(cfg.generate(new KeyMM))
  gen(cfg.generate(new LedMatrix))
  gen(cfg1.generate(new PS2Tx))
  gen(cfg1.generate(new PS2Rx))
}
