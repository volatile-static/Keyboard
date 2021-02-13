// Generator : SpinalHDL v1.4.3    git head : 222a7d41db5dd41b691a2f16f43476eafabeb170
// Component : LedMatrix
// Git hash  : 815874f99d43f50e40e2c09e5cafdb6f3a05db5a



module LedMatrix (
  output reg [34:0]   io_col,
  output     [5:0]    io_row,
  input               bus_write,
  input      [6:0]    bus_address,
  input      [31:0]   bus_writeData,
  input               clk,
  input               reset
);
  wire       [0:0]    _zz_112;
  wire       [7:0]    _zz_113;
  wire       [0:0]    _zz_114;
  wire       [2:0]    _zz_115;
  wire       [5:0]    _zz_116;
  wire                _zz_117;
  wire       [0:0]    _zz_118;
  wire       [64:0]   _zz_119;
  wire                _zz_120;
  wire       [0:0]    _zz_121;
  wire       [56:0]   _zz_122;
  wire                _zz_123;
  wire       [0:0]    _zz_124;
  wire       [48:0]   _zz_125;
  wire                _zz_126;
  wire       [0:0]    _zz_127;
  wire       [40:0]   _zz_128;
  wire                _zz_129;
  wire       [0:0]    _zz_130;
  wire       [32:0]   _zz_131;
  wire                _zz_132;
  wire       [0:0]    _zz_133;
  wire       [24:0]   _zz_134;
  wire                _zz_135;
  wire       [0:0]    _zz_136;
  wire       [16:0]   _zz_137;
  wire                _zz_138;
  wire       [0:0]    _zz_139;
  wire       [8:0]    _zz_140;
  wire                _zz_141;
  wire       [0:0]    _zz_142;
  wire       [0:0]    _zz_143;
  wire       [7:0]    regs_ice_0;
  wire       [7:0]    regs_ice_1;
  wire       [7:0]    regs_ice_2;
  wire       [7:0]    regs_ice_3;
  wire       [7:0]    regs_ice_4;
  wire       [7:0]    regs_ice_5;
  wire       [7:0]    regs_ice_6;
  wire       [7:0]    regs_ice_7;
  wire       [7:0]    regs_ice_8;
  wire       [7:0]    regs_ice_9;
  wire       [7:0]    regs_ice_10;
  wire       [7:0]    regs_ice_11;
  wire       [7:0]    regs_ice_12;
  wire       [7:0]    regs_ice_13;
  wire       [7:0]    regs_ice_14;
  wire       [7:0]    regs_ice_15;
  wire       [7:0]    regs_ice_16;
  wire       [7:0]    regs_ice_17;
  wire       [7:0]    regs_ice_18;
  wire       [7:0]    regs_ice_19;
  wire       [7:0]    regs_ice_20;
  wire       [7:0]    regs_ice_21;
  wire       [7:0]    regs_ice_22;
  wire       [7:0]    regs_ice_23;
  wire       [7:0]    regs_ice_24;
  wire       [7:0]    regs_ice_25;
  wire       [7:0]    regs_ice_26;
  wire       [7:0]    regs_ice_27;
  wire       [7:0]    regs_ice_28;
  wire       [7:0]    regs_ice_29;
  wire       [7:0]    regs_ice_30;
  wire       [7:0]    regs_ice_31;
  wire       [7:0]    regs_ice_32;
  wire       [7:0]    regs_ice_33;
  wire       [7:0]    regs_ice_34;
  wire       [7:0]    regs_ice_35;
  wire       [7:0]    regs_ice_36;
  wire       [7:0]    regs_ice_37;
  wire       [7:0]    regs_ice_38;
  wire       [7:0]    regs_ice_39;
  wire       [7:0]    regs_ice_40;
  wire       [7:0]    regs_ice_41;
  wire       [7:0]    regs_ice_42;
  wire       [7:0]    regs_ice_43;
  wire       [7:0]    regs_ice_44;
  wire       [7:0]    regs_ice_45;
  wire       [7:0]    regs_ice_46;
  wire       [7:0]    regs_ice_47;
  wire       [7:0]    regs_ice_48;
  wire       [7:0]    regs_ice_49;
  wire       [7:0]    regs_ice_50;
  wire       [7:0]    regs_ice_51;
  wire       [7:0]    regs_ice_52;
  wire       [7:0]    regs_ice_53;
  wire       [7:0]    regs_ice_54;
  wire       [7:0]    regs_ice_55;
  wire       [7:0]    regs_ice_56;
  wire       [7:0]    regs_ice_57;
  wire       [7:0]    regs_ice_58;
  wire       [7:0]    regs_ice_59;
  wire       [7:0]    regs_ice_60;
  wire       [7:0]    regs_ice_61;
  wire       [7:0]    regs_ice_62;
  wire       [7:0]    regs_ice_63;
  wire       [7:0]    regs_ice_64;
  wire       [7:0]    regs_ice_65;
  wire       [7:0]    regs_ice_66;
  wire       [7:0]    regs_ice_67;
  wire       [7:0]    regs_ice_68;
  wire       [7:0]    regs_ice_69;
  wire       [7:0]    regs_ice_70;
  wire       [7:0]    regs_ice_71;
  wire       [7:0]    regs_ice_72;
  wire       [15:0]   regs_fn;
  wire       [7:0]    regs_locks_0;
  wire       [7:0]    regs_locks_1;
  wire       [7:0]    regs_locks_2;
  wire       [23:0]   regs_rgb_0;
  wire       [23:0]   regs_rgb_1;
  wire       [23:0]   regs_rgb_2;
  wire       [23:0]   regs_rgb_3;
  wire       [23:0]   regs_rgb_4;
  wire       [23:0]   regs_rgb_5;
  wire       [23:0]   regs_rgb_6;
  wire       [23:0]   regs_rgb_7;
  wire       [23:0]   regs_rgb_8;
  wire       [23:0]   regs_rgb_9;
  wire       [23:0]   regs_rgb_10;
  wire       [23:0]   regs_rgb_11;
  wire       [23:0]   regs_rgb_12;
  wire       [23:0]   regs_rgb_13;
  wire       [23:0]   regs_rgb_14;
  wire       [23:0]   regs_rgb_15;
  wire       [23:0]   regs_rgb_16;
  wire       [23:0]   regs_rgb_17;
  wire       [23:0]   regs_rgb_18;
  wire       [23:0]   regs_rgb_19;
  wire       [23:0]   regs_rgb_20;
  wire       [23:0]   regs_rgb_21;
  wire       [23:0]   regs_rgb_22;
  wire       [23:0]   regs_rgb_23;
  wire       [23:0]   regs_rgb_24;
  wire       [23:0]   regs_rgb_25;
  wire       [23:0]   regs_rgb_26;
  wire       [23:0]   regs_rgb_27;
  wire       [23:0]   regs_rgb_28;
  wire       [23:0]   regs_rgb_29;
  reg        [31:0]   _zz_1;
  reg        [31:0]   _zz_2;
  reg        [31:0]   _zz_3;
  reg        [31:0]   _zz_4;
  reg        [31:0]   _zz_5;
  reg        [31:0]   _zz_6;
  reg        [31:0]   _zz_7;
  reg        [31:0]   _zz_8;
  reg        [31:0]   _zz_9;
  reg        [31:0]   _zz_10;
  reg        [31:0]   _zz_11;
  reg        [31:0]   _zz_12;
  reg        [31:0]   _zz_13;
  reg        [31:0]   _zz_14;
  reg        [31:0]   _zz_15;
  reg        [31:0]   _zz_16;
  reg        [31:0]   _zz_17;
  reg        [31:0]   _zz_18;
  reg        [31:0]   _zz_19;
  reg        [31:0]   _zz_20;
  reg        [31:0]   _zz_21;
  reg        [31:0]   _zz_22;
  reg        [31:0]   _zz_23;
  reg        [31:0]   _zz_24;
  reg        [31:0]   _zz_25;
  reg        [31:0]   _zz_26;
  reg        [31:0]   _zz_27;
  reg        [31:0]   _zz_28;
  reg        [31:0]   _zz_29;
  reg        [31:0]   _zz_30;
  reg        [31:0]   _zz_31;
  reg        [31:0]   _zz_32;
  reg        [31:0]   _zz_33;
  reg        [31:0]   _zz_34;
  reg        [31:0]   _zz_35;
  reg        [31:0]   _zz_36;
  reg        [31:0]   _zz_37;
  reg        [31:0]   _zz_38;
  reg        [31:0]   _zz_39;
  reg        [31:0]   _zz_40;
  reg        [31:0]   _zz_41;
  reg        [31:0]   _zz_42;
  reg        [31:0]   _zz_43;
  reg        [31:0]   _zz_44;
  reg        [31:0]   _zz_45;
  reg        [31:0]   _zz_46;
  reg        [31:0]   _zz_47;
  reg        [31:0]   _zz_48;
  reg        [31:0]   _zz_49;
  reg        [31:0]   _zz_50;
  reg        [31:0]   _zz_51;
  reg        [31:0]   _zz_52;
  reg        [31:0]   _zz_53;
  reg        [31:0]   _zz_54;
  reg        [31:0]   _zz_55;
  reg        [31:0]   _zz_56;
  reg        [31:0]   _zz_57;
  reg        [31:0]   _zz_58;
  reg        [31:0]   _zz_59;
  reg        [31:0]   _zz_60;
  reg        [31:0]   _zz_61;
  reg        [31:0]   _zz_62;
  reg        [31:0]   _zz_63;
  reg        [31:0]   _zz_64;
  reg        [31:0]   _zz_65;
  reg        [31:0]   _zz_66;
  reg        [31:0]   _zz_67;
  reg        [31:0]   _zz_68;
  reg        [31:0]   _zz_69;
  reg        [31:0]   _zz_70;
  reg        [31:0]   _zz_71;
  reg        [31:0]   _zz_72;
  reg        [31:0]   _zz_73;
  reg        [31:0]   _zz_74;
  reg        [31:0]   _zz_75;
  reg        [31:0]   _zz_76;
  reg        [31:0]   _zz_77;
  reg        [31:0]   _zz_78;
  reg        [31:0]   _zz_79;
  reg        [31:0]   _zz_80;
  reg        [31:0]   _zz_81;
  reg        [31:0]   _zz_82;
  reg        [31:0]   _zz_83;
  reg        [31:0]   _zz_84;
  reg        [31:0]   _zz_85;
  reg        [31:0]   _zz_86;
  reg        [31:0]   _zz_87;
  reg        [31:0]   _zz_88;
  reg        [31:0]   _zz_89;
  reg        [31:0]   _zz_90;
  reg        [31:0]   _zz_91;
  reg        [31:0]   _zz_92;
  reg        [31:0]   _zz_93;
  reg        [31:0]   _zz_94;
  reg        [31:0]   _zz_95;
  reg        [31:0]   _zz_96;
  reg        [31:0]   _zz_97;
  reg        [31:0]   _zz_98;
  reg        [31:0]   _zz_99;
  reg        [31:0]   _zz_100;
  reg        [31:0]   _zz_101;
  reg        [31:0]   _zz_102;
  reg        [31:0]   _zz_103;
  reg        [31:0]   _zz_104;
  reg        [31:0]   _zz_105;
  reg        [31:0]   _zz_106;
  reg        [31:0]   _zz_107;
  reg        [167:0]  ledSta;
  reg        [11:0]   _zz_108;
  wire                _zz_109;
  reg                 _zz_109_regNext;
  wire                pwm_pwmCnt_willIncrement;
  wire                pwm_pwmCnt_willClear;
  reg        [7:0]    pwm_pwmCnt_valueNext;
  reg        [7:0]    pwm_pwmCnt_value;
  wire                pwm_pwmCnt_willOverflowIfInc;
  wire                pwm_pwmCnt_willOverflow;
  reg        [9:0]    _zz_110;
  wire                _zz_111;
  reg                 _zz_111_regNext;
  wire                scanner_rowCnt_willIncrement;
  wire                scanner_rowCnt_willClear;
  reg        [2:0]    scanner_rowCnt_valueNext;
  reg        [2:0]    scanner_rowCnt_value;
  wire                scanner_rowCnt_willOverflowIfInc;
  wire                scanner_rowCnt_willOverflow;

  assign _zz_112 = pwm_pwmCnt_willIncrement;
  assign _zz_113 = {7'd0, _zz_112};
  assign _zz_114 = scanner_rowCnt_willIncrement;
  assign _zz_115 = {2'd0, _zz_114};
  assign _zz_116 = (6'h01 <<< scanner_rowCnt_value);
  assign _zz_117 = (regs_ice_66 < pwm_pwmCnt_value);
  assign _zz_118 = (regs_ice_65 < pwm_pwmCnt_value);
  assign _zz_119 = {(regs_ice_64 < pwm_pwmCnt_value),{(regs_ice_63 < pwm_pwmCnt_value),{(regs_ice_62 < pwm_pwmCnt_value),{(regs_ice_61 < pwm_pwmCnt_value),{(regs_ice_60 < pwm_pwmCnt_value),{(regs_ice_59 < pwm_pwmCnt_value),{_zz_120,{_zz_121,_zz_122}}}}}}}};
  assign _zz_120 = (regs_ice_58 < pwm_pwmCnt_value);
  assign _zz_121 = (regs_ice_57 < pwm_pwmCnt_value);
  assign _zz_122 = {(regs_ice_56 < pwm_pwmCnt_value),{(regs_ice_55 < pwm_pwmCnt_value),{(regs_ice_54 < pwm_pwmCnt_value),{(regs_ice_53 < pwm_pwmCnt_value),{(regs_ice_52 < pwm_pwmCnt_value),{(regs_ice_51 < pwm_pwmCnt_value),{_zz_123,{_zz_124,_zz_125}}}}}}}};
  assign _zz_123 = (regs_ice_50 < pwm_pwmCnt_value);
  assign _zz_124 = (regs_ice_49 < pwm_pwmCnt_value);
  assign _zz_125 = {(regs_ice_48 < pwm_pwmCnt_value),{(regs_ice_47 < pwm_pwmCnt_value),{(regs_ice_46 < pwm_pwmCnt_value),{(regs_ice_45 < pwm_pwmCnt_value),{(regs_ice_44 < pwm_pwmCnt_value),{(regs_ice_43 < pwm_pwmCnt_value),{_zz_126,{_zz_127,_zz_128}}}}}}}};
  assign _zz_126 = (regs_ice_42 < pwm_pwmCnt_value);
  assign _zz_127 = (regs_ice_41 < pwm_pwmCnt_value);
  assign _zz_128 = {(regs_ice_40 < pwm_pwmCnt_value),{(regs_ice_39 < pwm_pwmCnt_value),{(regs_ice_38 < pwm_pwmCnt_value),{(regs_ice_37 < pwm_pwmCnt_value),{(regs_ice_36 < pwm_pwmCnt_value),{(regs_ice_35 < pwm_pwmCnt_value),{_zz_129,{_zz_130,_zz_131}}}}}}}};
  assign _zz_129 = (regs_ice_34 < pwm_pwmCnt_value);
  assign _zz_130 = (regs_ice_33 < pwm_pwmCnt_value);
  assign _zz_131 = {(regs_ice_32 < pwm_pwmCnt_value),{(regs_ice_31 < pwm_pwmCnt_value),{(regs_ice_30 < pwm_pwmCnt_value),{(regs_ice_29 < pwm_pwmCnt_value),{(regs_ice_28 < pwm_pwmCnt_value),{(regs_ice_27 < pwm_pwmCnt_value),{_zz_132,{_zz_133,_zz_134}}}}}}}};
  assign _zz_132 = (regs_ice_26 < pwm_pwmCnt_value);
  assign _zz_133 = (regs_ice_25 < pwm_pwmCnt_value);
  assign _zz_134 = {(regs_ice_24 < pwm_pwmCnt_value),{(regs_ice_23 < pwm_pwmCnt_value),{(regs_ice_22 < pwm_pwmCnt_value),{(regs_ice_21 < pwm_pwmCnt_value),{(regs_ice_20 < pwm_pwmCnt_value),{(regs_ice_19 < pwm_pwmCnt_value),{_zz_135,{_zz_136,_zz_137}}}}}}}};
  assign _zz_135 = (regs_ice_18 < pwm_pwmCnt_value);
  assign _zz_136 = (regs_ice_17 < pwm_pwmCnt_value);
  assign _zz_137 = {(regs_ice_16 < pwm_pwmCnt_value),{(regs_ice_15 < pwm_pwmCnt_value),{(regs_ice_14 < pwm_pwmCnt_value),{(regs_ice_13 < pwm_pwmCnt_value),{(regs_ice_12 < pwm_pwmCnt_value),{(regs_ice_11 < pwm_pwmCnt_value),{_zz_138,{_zz_139,_zz_140}}}}}}}};
  assign _zz_138 = (regs_ice_10 < pwm_pwmCnt_value);
  assign _zz_139 = (regs_ice_9 < pwm_pwmCnt_value);
  assign _zz_140 = {(regs_ice_8 < pwm_pwmCnt_value),{(regs_ice_7 < pwm_pwmCnt_value),{(regs_ice_6 < pwm_pwmCnt_value),{(regs_ice_5 < pwm_pwmCnt_value),{(regs_ice_4 < pwm_pwmCnt_value),{(regs_ice_3 < pwm_pwmCnt_value),{_zz_141,{_zz_142,_zz_143}}}}}}}};
  assign _zz_141 = (regs_ice_2 < pwm_pwmCnt_value);
  assign _zz_142 = (regs_ice_1 < pwm_pwmCnt_value);
  assign _zz_143 = (regs_ice_0 < pwm_pwmCnt_value);
  assign regs_ice_0 = _zz_1[7:0];
  assign regs_ice_1 = _zz_2[7:0];
  assign regs_ice_2 = _zz_3[7:0];
  assign regs_ice_3 = _zz_4[7:0];
  assign regs_ice_4 = _zz_5[7:0];
  assign regs_ice_5 = _zz_6[7:0];
  assign regs_ice_6 = _zz_7[7:0];
  assign regs_ice_7 = _zz_8[7:0];
  assign regs_ice_8 = _zz_9[7:0];
  assign regs_ice_9 = _zz_10[7:0];
  assign regs_ice_10 = _zz_11[7:0];
  assign regs_ice_11 = _zz_12[7:0];
  assign regs_ice_12 = _zz_13[7:0];
  assign regs_ice_13 = _zz_14[7:0];
  assign regs_ice_14 = _zz_15[7:0];
  assign regs_ice_15 = _zz_16[7:0];
  assign regs_ice_16 = _zz_17[7:0];
  assign regs_ice_17 = _zz_18[7:0];
  assign regs_ice_18 = _zz_19[7:0];
  assign regs_ice_19 = _zz_20[7:0];
  assign regs_ice_20 = _zz_21[7:0];
  assign regs_ice_21 = _zz_22[7:0];
  assign regs_ice_22 = _zz_23[7:0];
  assign regs_ice_23 = _zz_24[7:0];
  assign regs_ice_24 = _zz_25[7:0];
  assign regs_ice_25 = _zz_26[7:0];
  assign regs_ice_26 = _zz_27[7:0];
  assign regs_ice_27 = _zz_28[7:0];
  assign regs_ice_28 = _zz_29[7:0];
  assign regs_ice_29 = _zz_30[7:0];
  assign regs_ice_30 = _zz_31[7:0];
  assign regs_ice_31 = _zz_32[7:0];
  assign regs_ice_32 = _zz_33[7:0];
  assign regs_ice_33 = _zz_34[7:0];
  assign regs_ice_34 = _zz_35[7:0];
  assign regs_ice_35 = _zz_36[7:0];
  assign regs_ice_36 = _zz_37[7:0];
  assign regs_ice_37 = _zz_38[7:0];
  assign regs_ice_38 = _zz_39[7:0];
  assign regs_ice_39 = _zz_40[7:0];
  assign regs_ice_40 = _zz_41[7:0];
  assign regs_ice_41 = _zz_42[7:0];
  assign regs_ice_42 = _zz_43[7:0];
  assign regs_ice_43 = _zz_44[7:0];
  assign regs_ice_44 = _zz_45[7:0];
  assign regs_ice_45 = _zz_46[7:0];
  assign regs_ice_46 = _zz_47[7:0];
  assign regs_ice_47 = _zz_48[7:0];
  assign regs_ice_48 = _zz_49[7:0];
  assign regs_ice_49 = _zz_50[7:0];
  assign regs_ice_50 = _zz_51[7:0];
  assign regs_ice_51 = _zz_52[7:0];
  assign regs_ice_52 = _zz_53[7:0];
  assign regs_ice_53 = _zz_54[7:0];
  assign regs_ice_54 = _zz_55[7:0];
  assign regs_ice_55 = _zz_56[7:0];
  assign regs_ice_56 = _zz_57[7:0];
  assign regs_ice_57 = _zz_58[7:0];
  assign regs_ice_58 = _zz_59[7:0];
  assign regs_ice_59 = _zz_60[7:0];
  assign regs_ice_60 = _zz_61[7:0];
  assign regs_ice_61 = _zz_62[7:0];
  assign regs_ice_62 = _zz_63[7:0];
  assign regs_ice_63 = _zz_64[7:0];
  assign regs_ice_64 = _zz_65[7:0];
  assign regs_ice_65 = _zz_66[7:0];
  assign regs_ice_66 = _zz_67[7:0];
  assign regs_ice_67 = _zz_68[7:0];
  assign regs_ice_68 = _zz_69[7:0];
  assign regs_ice_69 = _zz_70[7:0];
  assign regs_ice_70 = _zz_71[7:0];
  assign regs_ice_71 = _zz_72[7:0];
  assign regs_ice_72 = _zz_73[7:0];
  assign regs_fn = _zz_74[15:0];
  assign regs_rgb_0 = _zz_75[23:0];
  assign regs_rgb_1 = _zz_76[23:0];
  assign regs_rgb_2 = _zz_77[23:0];
  assign regs_rgb_3 = _zz_78[23:0];
  assign regs_rgb_4 = _zz_79[23:0];
  assign regs_rgb_5 = _zz_80[23:0];
  assign regs_rgb_6 = _zz_81[23:0];
  assign regs_rgb_7 = _zz_82[23:0];
  assign regs_rgb_8 = _zz_83[23:0];
  assign regs_rgb_9 = _zz_84[23:0];
  assign regs_rgb_10 = _zz_85[23:0];
  assign regs_rgb_11 = _zz_86[23:0];
  assign regs_rgb_12 = _zz_87[23:0];
  assign regs_rgb_13 = _zz_88[23:0];
  assign regs_rgb_14 = _zz_89[23:0];
  assign regs_rgb_15 = _zz_90[23:0];
  assign regs_rgb_16 = _zz_91[23:0];
  assign regs_rgb_17 = _zz_92[23:0];
  assign regs_rgb_18 = _zz_93[23:0];
  assign regs_rgb_19 = _zz_94[23:0];
  assign regs_rgb_20 = _zz_95[23:0];
  assign regs_rgb_21 = _zz_96[23:0];
  assign regs_rgb_22 = _zz_97[23:0];
  assign regs_rgb_23 = _zz_98[23:0];
  assign regs_rgb_24 = _zz_99[23:0];
  assign regs_rgb_25 = _zz_100[23:0];
  assign regs_rgb_26 = _zz_101[23:0];
  assign regs_rgb_27 = _zz_102[23:0];
  assign regs_rgb_28 = _zz_103[23:0];
  assign regs_rgb_29 = _zz_104[23:0];
  assign regs_locks_0 = _zz_105[7:0];
  assign regs_locks_1 = _zz_106[7:0];
  assign regs_locks_2 = _zz_107[7:0];
  assign _zz_109 = (_zz_108 == 12'hf9f);
  assign pwm_pwmCnt_willClear = 1'b0;
  assign pwm_pwmCnt_willOverflowIfInc = (pwm_pwmCnt_value == 8'hff);
  assign pwm_pwmCnt_willOverflow = (pwm_pwmCnt_willOverflowIfInc && pwm_pwmCnt_willIncrement);
  always @ (*) begin
    pwm_pwmCnt_valueNext = (pwm_pwmCnt_value + _zz_113);
    if(pwm_pwmCnt_willClear)begin
      pwm_pwmCnt_valueNext = 8'h0;
    end
  end

  assign pwm_pwmCnt_willIncrement = 1'b1;
  always @ (*) begin
    ledSta[72 : 0] = {(regs_ice_72 < pwm_pwmCnt_value),{(regs_ice_71 < pwm_pwmCnt_value),{(regs_ice_70 < pwm_pwmCnt_value),{(regs_ice_69 < pwm_pwmCnt_value),{(regs_ice_68 < pwm_pwmCnt_value),{(regs_ice_67 < pwm_pwmCnt_value),{_zz_117,{_zz_118,_zz_119}}}}}}}};
    ledSta[74 : 73] = {(regs_fn[7 : 0] < pwm_pwmCnt_value),(regs_fn[15 : 8] < pwm_pwmCnt_value)};
    ledSta[77 : 75] = {(regs_rgb_0[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_0[15 : 8] < pwm_pwmCnt_value),(regs_rgb_0[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[80 : 78] = {(regs_rgb_1[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_1[15 : 8] < pwm_pwmCnt_value),(regs_rgb_1[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[83 : 81] = {(regs_rgb_2[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_2[15 : 8] < pwm_pwmCnt_value),(regs_rgb_2[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[86 : 84] = {(regs_rgb_3[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_3[15 : 8] < pwm_pwmCnt_value),(regs_rgb_3[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[89 : 87] = {(regs_rgb_4[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_4[15 : 8] < pwm_pwmCnt_value),(regs_rgb_4[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[92 : 90] = {(regs_rgb_5[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_5[15 : 8] < pwm_pwmCnt_value),(regs_rgb_5[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[95 : 93] = {(regs_rgb_6[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_6[15 : 8] < pwm_pwmCnt_value),(regs_rgb_6[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[98 : 96] = {(regs_rgb_7[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_7[15 : 8] < pwm_pwmCnt_value),(regs_rgb_7[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[101 : 99] = {(regs_rgb_8[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_8[15 : 8] < pwm_pwmCnt_value),(regs_rgb_8[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[104 : 102] = {(regs_rgb_9[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_9[15 : 8] < pwm_pwmCnt_value),(regs_rgb_9[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[107 : 105] = {(regs_rgb_10[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_10[15 : 8] < pwm_pwmCnt_value),(regs_rgb_10[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[110 : 108] = {(regs_rgb_11[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_11[15 : 8] < pwm_pwmCnt_value),(regs_rgb_11[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[113 : 111] = {(regs_rgb_12[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_12[15 : 8] < pwm_pwmCnt_value),(regs_rgb_12[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[116 : 114] = {(regs_rgb_13[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_13[15 : 8] < pwm_pwmCnt_value),(regs_rgb_13[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[119 : 117] = {(regs_rgb_14[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_14[15 : 8] < pwm_pwmCnt_value),(regs_rgb_14[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[122 : 120] = {(regs_rgb_15[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_15[15 : 8] < pwm_pwmCnt_value),(regs_rgb_15[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[125 : 123] = {(regs_rgb_16[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_16[15 : 8] < pwm_pwmCnt_value),(regs_rgb_16[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[128 : 126] = {(regs_rgb_17[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_17[15 : 8] < pwm_pwmCnt_value),(regs_rgb_17[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[131 : 129] = {(regs_rgb_18[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_18[15 : 8] < pwm_pwmCnt_value),(regs_rgb_18[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[134 : 132] = {(regs_rgb_19[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_19[15 : 8] < pwm_pwmCnt_value),(regs_rgb_19[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[137 : 135] = {(regs_rgb_20[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_20[15 : 8] < pwm_pwmCnt_value),(regs_rgb_20[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[140 : 138] = {(regs_rgb_21[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_21[15 : 8] < pwm_pwmCnt_value),(regs_rgb_21[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[143 : 141] = {(regs_rgb_22[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_22[15 : 8] < pwm_pwmCnt_value),(regs_rgb_22[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[146 : 144] = {(regs_rgb_23[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_23[15 : 8] < pwm_pwmCnt_value),(regs_rgb_23[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[149 : 147] = {(regs_rgb_24[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_24[15 : 8] < pwm_pwmCnt_value),(regs_rgb_24[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[152 : 150] = {(regs_rgb_25[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_25[15 : 8] < pwm_pwmCnt_value),(regs_rgb_25[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[155 : 153] = {(regs_rgb_26[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_26[15 : 8] < pwm_pwmCnt_value),(regs_rgb_26[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[158 : 156] = {(regs_rgb_27[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_27[15 : 8] < pwm_pwmCnt_value),(regs_rgb_27[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[161 : 159] = {(regs_rgb_28[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_28[15 : 8] < pwm_pwmCnt_value),(regs_rgb_28[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[164 : 162] = {(regs_rgb_29[7 : 0] < pwm_pwmCnt_value),{(regs_rgb_29[15 : 8] < pwm_pwmCnt_value),(regs_rgb_29[23 : 16] < pwm_pwmCnt_value)}};
    ledSta[167 : 165] = {(regs_locks_2 < pwm_pwmCnt_value),{(regs_locks_1 < pwm_pwmCnt_value),(regs_locks_0 < pwm_pwmCnt_value)}};
  end

  assign _zz_111 = (_zz_110 == 10'h299);
  assign scanner_rowCnt_willClear = 1'b0;
  assign scanner_rowCnt_willOverflowIfInc = (scanner_rowCnt_value == 3'b101);
  assign scanner_rowCnt_willOverflow = (scanner_rowCnt_willOverflowIfInc && scanner_rowCnt_willIncrement);
  always @ (*) begin
    if(scanner_rowCnt_willOverflow)begin
      scanner_rowCnt_valueNext = 3'b000;
    end else begin
      scanner_rowCnt_valueNext = (scanner_rowCnt_value + _zz_115);
    end
    if(scanner_rowCnt_willClear)begin
      scanner_rowCnt_valueNext = 3'b000;
    end
  end

  assign scanner_rowCnt_willIncrement = 1'b1;
  assign io_row = (~ _zz_116);
  always @ (*) begin
    io_col = (~ 35'h0);
    case(scanner_rowCnt_value)
      3'b000 : begin
        io_col[0] = ledSta[0];
        io_col[13 : 2] = ledSta[12 : 1];
        io_col[22 : 14] = ledSta[83 : 75];
        io_col[28] = ledSta[165];
        io_col[31] = ledSta[166];
        io_col[34] = ledSta[167];
      end
      3'b001 : begin
        io_col[13 : 0] = ledSta[26 : 13];
        io_col[34 : 14] = ledSta[104 : 84];
      end
      3'b010 : begin
        io_col[13 : 0] = ledSta[40 : 27];
        io_col[34 : 14] = ledSta[125 : 105];
      end
      3'b011 : begin
        io_col[11 : 0] = ledSta[52 : 41];
        io_col[13] = ledSta[53];
        io_col[31 : 23] = ledSta[134 : 126];
      end
      3'b100 : begin
        io_col[10 : 0] = ledSta[64 : 54];
        io_col[13] = ledSta[65];
        io_col[19 : 17] = ledSta[137 : 135];
        io_col[31 : 23] = ledSta[146 : 138];
      end
      3'b101 : begin
        io_col[2 : 0] = ledSta[68 : 66];
        io_col[5] = ledSta[69];
        io_col[10 : 9] = ledSta[71 : 70];
        io_col[12 : 11] = ledSta[74 : 73];
        io_col[25 : 13] = {ledSta[158 : 147],ledSta[72]};
        io_col[34 : 29] = ledSta[164 : 159];
      end
      default : begin
      end
    endcase
  end

  always @ (posedge clk or posedge reset) begin
    if (reset) begin
      _zz_1 <= 32'h0;
      _zz_2 <= 32'h0;
      _zz_3 <= 32'h0;
      _zz_4 <= 32'h0;
      _zz_5 <= 32'h0;
      _zz_6 <= 32'h0;
      _zz_7 <= 32'h0;
      _zz_8 <= 32'h0;
      _zz_9 <= 32'h0;
      _zz_10 <= 32'h0;
      _zz_11 <= 32'h0;
      _zz_12 <= 32'h0;
      _zz_13 <= 32'h0;
      _zz_14 <= 32'h0;
      _zz_15 <= 32'h0;
      _zz_16 <= 32'h0;
      _zz_17 <= 32'h0;
      _zz_18 <= 32'h0;
      _zz_19 <= 32'h0;
      _zz_20 <= 32'h0;
      _zz_21 <= 32'h0;
      _zz_22 <= 32'h0;
      _zz_23 <= 32'h0;
      _zz_24 <= 32'h0;
      _zz_25 <= 32'h0;
      _zz_26 <= 32'h0;
      _zz_27 <= 32'h0;
      _zz_28 <= 32'h0;
      _zz_29 <= 32'h0;
      _zz_30 <= 32'h0;
      _zz_31 <= 32'h0;
      _zz_32 <= 32'h0;
      _zz_33 <= 32'h0;
      _zz_34 <= 32'h0;
      _zz_35 <= 32'h0;
      _zz_36 <= 32'h0;
      _zz_37 <= 32'h0;
      _zz_38 <= 32'h0;
      _zz_39 <= 32'h0;
      _zz_40 <= 32'h0;
      _zz_41 <= 32'h0;
      _zz_42 <= 32'h0;
      _zz_43 <= 32'h0;
      _zz_44 <= 32'h0;
      _zz_45 <= 32'h0;
      _zz_46 <= 32'h0;
      _zz_47 <= 32'h0;
      _zz_48 <= 32'h0;
      _zz_49 <= 32'h0;
      _zz_50 <= 32'h0;
      _zz_51 <= 32'h0;
      _zz_52 <= 32'h0;
      _zz_53 <= 32'h0;
      _zz_54 <= 32'h0;
      _zz_55 <= 32'h0;
      _zz_56 <= 32'h0;
      _zz_57 <= 32'h0;
      _zz_58 <= 32'h0;
      _zz_59 <= 32'h0;
      _zz_60 <= 32'h0;
      _zz_61 <= 32'h0;
      _zz_62 <= 32'h0;
      _zz_63 <= 32'h0;
      _zz_64 <= 32'h0;
      _zz_65 <= 32'h0;
      _zz_66 <= 32'h0;
      _zz_67 <= 32'h0;
      _zz_68 <= 32'h0;
      _zz_69 <= 32'h0;
      _zz_70 <= 32'h0;
      _zz_71 <= 32'h0;
      _zz_72 <= 32'h0;
      _zz_73 <= 32'h0;
      _zz_74 <= 32'h0;
      _zz_75 <= 32'h0;
      _zz_76 <= 32'h0;
      _zz_77 <= 32'h0;
      _zz_78 <= 32'h0;
      _zz_79 <= 32'h0;
      _zz_80 <= 32'h0;
      _zz_81 <= 32'h0;
      _zz_82 <= 32'h0;
      _zz_83 <= 32'h0;
      _zz_84 <= 32'h0;
      _zz_85 <= 32'h0;
      _zz_86 <= 32'h0;
      _zz_87 <= 32'h0;
      _zz_88 <= 32'h0;
      _zz_89 <= 32'h0;
      _zz_90 <= 32'h0;
      _zz_91 <= 32'h0;
      _zz_92 <= 32'h0;
      _zz_93 <= 32'h0;
      _zz_94 <= 32'h0;
      _zz_95 <= 32'h0;
      _zz_96 <= 32'h0;
      _zz_97 <= 32'h0;
      _zz_98 <= 32'h0;
      _zz_99 <= 32'h0;
      _zz_100 <= 32'h0;
      _zz_101 <= 32'h0;
      _zz_102 <= 32'h0;
      _zz_103 <= 32'h0;
      _zz_104 <= 32'h0;
      _zz_105 <= 32'h0;
      _zz_106 <= 32'h0;
      _zz_107 <= 32'h0;
      _zz_108 <= 12'h0;
      _zz_109_regNext <= 1'b0;
      _zz_110 <= 10'h0;
      _zz_111_regNext <= 1'b0;
    end else begin
      if((bus_write && (bus_address == 7'h0)))begin
        _zz_1 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h01)))begin
        _zz_2 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h02)))begin
        _zz_3 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h03)))begin
        _zz_4 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h04)))begin
        _zz_5 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h05)))begin
        _zz_6 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h06)))begin
        _zz_7 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h07)))begin
        _zz_8 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h08)))begin
        _zz_9 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h09)))begin
        _zz_10 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0a)))begin
        _zz_11 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0b)))begin
        _zz_12 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0c)))begin
        _zz_13 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0d)))begin
        _zz_14 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0e)))begin
        _zz_15 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h0f)))begin
        _zz_16 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h10)))begin
        _zz_17 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h11)))begin
        _zz_18 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h12)))begin
        _zz_19 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h13)))begin
        _zz_20 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h14)))begin
        _zz_21 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h15)))begin
        _zz_22 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h16)))begin
        _zz_23 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h17)))begin
        _zz_24 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h18)))begin
        _zz_25 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h19)))begin
        _zz_26 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1a)))begin
        _zz_27 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1b)))begin
        _zz_28 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1c)))begin
        _zz_29 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1d)))begin
        _zz_30 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1e)))begin
        _zz_31 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h1f)))begin
        _zz_32 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h20)))begin
        _zz_33 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h21)))begin
        _zz_34 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h22)))begin
        _zz_35 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h23)))begin
        _zz_36 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h24)))begin
        _zz_37 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h25)))begin
        _zz_38 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h26)))begin
        _zz_39 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h27)))begin
        _zz_40 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h28)))begin
        _zz_41 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h29)))begin
        _zz_42 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2a)))begin
        _zz_43 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2b)))begin
        _zz_44 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2c)))begin
        _zz_45 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2d)))begin
        _zz_46 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2e)))begin
        _zz_47 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h2f)))begin
        _zz_48 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h30)))begin
        _zz_49 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h31)))begin
        _zz_50 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h32)))begin
        _zz_51 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h33)))begin
        _zz_52 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h34)))begin
        _zz_53 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h35)))begin
        _zz_54 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h36)))begin
        _zz_55 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h37)))begin
        _zz_56 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h38)))begin
        _zz_57 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h39)))begin
        _zz_58 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3a)))begin
        _zz_59 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3b)))begin
        _zz_60 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3c)))begin
        _zz_61 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3d)))begin
        _zz_62 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3e)))begin
        _zz_63 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h3f)))begin
        _zz_64 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h40)))begin
        _zz_65 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h41)))begin
        _zz_66 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h42)))begin
        _zz_67 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h43)))begin
        _zz_68 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h44)))begin
        _zz_69 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h45)))begin
        _zz_70 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h46)))begin
        _zz_71 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h47)))begin
        _zz_72 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h48)))begin
        _zz_73 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h49)))begin
        _zz_74 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4a)))begin
        _zz_75 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4b)))begin
        _zz_76 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4c)))begin
        _zz_77 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4d)))begin
        _zz_78 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4e)))begin
        _zz_79 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h4f)))begin
        _zz_80 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h50)))begin
        _zz_81 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h51)))begin
        _zz_82 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h52)))begin
        _zz_83 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h53)))begin
        _zz_84 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h54)))begin
        _zz_85 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h55)))begin
        _zz_86 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h56)))begin
        _zz_87 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h57)))begin
        _zz_88 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h58)))begin
        _zz_89 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h59)))begin
        _zz_90 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5a)))begin
        _zz_91 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5b)))begin
        _zz_92 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5c)))begin
        _zz_93 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5d)))begin
        _zz_94 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5e)))begin
        _zz_95 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h5f)))begin
        _zz_96 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h60)))begin
        _zz_97 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h61)))begin
        _zz_98 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h62)))begin
        _zz_99 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h63)))begin
        _zz_100 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h64)))begin
        _zz_101 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h65)))begin
        _zz_102 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h66)))begin
        _zz_103 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h67)))begin
        _zz_104 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h68)))begin
        _zz_105 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h69)))begin
        _zz_106 <= bus_writeData;
      end
      if((bus_write && (bus_address == 7'h6a)))begin
        _zz_107 <= bus_writeData;
      end
      _zz_108 <= (_zz_108 + 12'h001);
      if(_zz_109)begin
        _zz_108 <= 12'h0;
      end
      _zz_109_regNext <= _zz_109;
      _zz_110 <= (_zz_110 + 10'h001);
      if(_zz_111)begin
        _zz_110 <= 10'h0;
      end
      _zz_111_regNext <= _zz_111;
    end
  end

  always @ (posedge clk or posedge reset) begin
    if (reset) begin
      pwm_pwmCnt_value <= 8'h0;
    end else begin
      if(_zz_109_regNext) begin
        pwm_pwmCnt_value <= pwm_pwmCnt_valueNext;
      end
    end
  end

  always @ (posedge clk or posedge reset) begin
    if (reset) begin
      scanner_rowCnt_value <= 3'b000;
    end else begin
      if(_zz_111_regNext) begin
        scanner_rowCnt_value <= scanner_rowCnt_valueNext;
      end
    end
  end


endmodule
