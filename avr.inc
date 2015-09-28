;avr for fasm
;(c) den_po, 2006
;http://justdanpo.cheb.ru

macro display_num x,y
{
  if x = 0
    display '0'
  else if x=7
    display '7'
  else if x=16
    display '16'
  else if x=23
    display '23'
  else if x=31
    display '31'
  else if x=63
    display '63'
  else if x=255
    display '255'
  else
    display 'unknown'
  end if

}

macro _checkvalue par*,reg*,lo*,hi*,odd
{
  if reg<lo | reg>hi
    display_num par,10
    display '-st parameter must be in range '
    display_num lo,10
    display '..'
    display_num hi,10
    display 13,10
    error
  end if
  if ~ odd eq
    if reg mod 2 = 1
      display_num par,10
      display '-st parameter must be odd'
      error
    end if
  end if
}

macro _shortjump address,cod
{
  if ((address-2-$)/2 < -64) | ((address-2-$)/2 > 63)
    display 'too long for short jump'
    error
  end if
  dw cod+ (0x7F and ((address-2-$)/2) )*8
}

macro _relativejump address,cod
{
  if ((address-2-$)/2 < -2048) | ((address-2-$)/2 > 2047)
    display 'too long for relative jump'
    error
  end if
  dw cod+ ((address-2-$)/2) and 0xFFF
}

macro _bigjump address,cod
{
  if 0x3FF800000 and address
    display 'address must be in 0..7FFFFF'
    error
  end if
  dw cod + \
      (0x3E0000 and (address/2) )/8192+ \
      (0x010000 and (address/2) )/65536
  dw 0xFFFF and (address/2)
}


macro org address
{
  org address*2
}

macro adc reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x1C00+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro add reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x0C00+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro adiw reg,val
{
  if ~ reg in <r24,XL,YL,ZL>
    display "want r24,XL/YL/ZL instead '",`reg,"'",13,10
    error
  end if
  _checkvalue 2,val,0,63
  dw 0x9600+((reg-r24)/2*16)+(0x30 and val)*4+(0x0F and val)
}

macro and reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x2000+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro andi reg,val
{
  _checkvalue 1,reg,16,31
  dw 0x7000+(0xF0 and val)*16+(0x0F and val)+(0x0F and reg)*16
}

macro asr reg
{
  _checkvalue 1,reg,0,31
  dw 0x9405+reg*16
}

macro bclr bit
{
  _checkvalue 1,bit,0,7
  dw 0x9488+(bit*16)
}

macro bld reg,bit
{
  _checkvalue 1,reg,0,31
  _checkvalue 2,bit,0,7
  dw 0xF800+reg*16+bit
}

macro brbc bit,address
{
  _checkvalue 1,bit,0,7
  if ((address-2-$)/2 < -64) | ((address-2-$)/2 > 63)
    display 'too long for short jump'
    error
  end if
  dw 0xF400+ (0x7F and ((address-2-$)/2) )*8+bit
}

macro brbs bit,address
{
  _checkvalue 1,bit,0,7
  if ((address-2-$)/2 < -64) | ((address-2-$)/2 > 63)
    display 'too long for short jump'
    error
  end if
  dw 0xF000+ (0x7F and ((address-2-$)/2) )*8+bit
}

macro brcc address { brbc 0,address }
macro brcs address { brbs 0,address }

macro break { dw 0x9598 }

macro breq address { brbs 1,address }
macro brge address { brbc 4,address }
macro brhc address { brbc 5,address }
macro brhs address { brbs 5,address }
macro brid address { brbc 7,address }
macro brie address { brbs 7,address }
macro brlo address { brbs 0,address }
macro brlt address { brbs 4,address }
macro brmi address { brbs 2,address }
macro brne address { brbc 1,address }
macro brpl address { brbc 2,address }
macro brsh address { brbc 0,address }
macro brtc address { brbc 6,address }
macro brts address { brbs 6,address }
macro brvc address { brbc 3,address }
macro brvs address { brbs 3,address }

macro bset bit
{
  _checkvalue 1,bit,0,7
  dw 0x9408+(bit*16)
}

macro bst reg,bit
{
  _checkvalue 1,reg,0,31
  _checkvalue 2,bit,0,7
  dw 0xFA00+reg*16+bit
}

macro call address
{
  _bigjump address,0x940E
}

macro cbi port,bit
{
  _checkvalue 1,port,0,31
  _checkvalue 2,bit,0,7
  dw 0x9800+port*8+bit
}

macro cbr reg,val
{
  andi reg,not val
}

macro clc { bclr 0 }
macro clh { bclr 5 }
macro cli { bclr 7 }
macro cln { bclr 2 }

macro clr reg
{
  eor reg,reg
}

macro cls { bclr 4 }
macro clt { bclr 6 }
macro clv { bclr 3 }
macro clz { bclr 1 }

macro com reg
{
  _checkvalue 1,reg,0,31
  dw 0x9400+reg*16
}

macro cp reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x1400+(0x10 and reg2 )*32+(0x0F and reg2 )+(0x1F and reg1 )*16
}

macro cpc reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x0400+(0x10 and reg2 )*32+(0x0F and reg2 )+(0x1F and reg1 )*16
}

macro cpi reg,val
{
  _checkvalue 1,reg,16,31
  dw 0x3000+(0xF0 and val )*16+(0x0F and val )+(0x0F and reg )*16
}

macro cpse reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x1000+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro dec reg
{
  _checkvalue 1,reg,0,31
  dw 0x940A+reg*16
}

macro eicall
{
  dw 0x9519
}

macro eijmp
{
  dw 0x9419
}

macro elpm reg,arg1
{
  if reg eq
    dw 0x95D8
  else
    local tempreg
    tempreg equ reg
    _checkvalue 1,tempreg,0,31

    local sign
    local srcreg
    local done

    done equ notyet

    match sr,arg1     \{
      srcreg equ sr
      sign equ none  \}
    match sr=+,arg1   \{
      srcreg equ sr
      sign equ plus  \}

    match =Z,srcreg
    \{
      match =plus ,sign  \\{ dw 0x9007+reg*16 \\}
      match =none ,sign  \\{ dw 0x9006+reg*16 \\}
      done equ
    \}
    if done eq notyet
      display "want Z[+] instead '",`arg1,"'",13,10
      exit
    end if
  end if
}

macro eor reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x2400+(0x10 and reg2 )*32+(0x0F and reg2 )+(0x1F and reg1 )*16
}

macro fmul reg1,reg2
{
  _checkvalue 1,reg1,16,23
  _checkvalue 2,reg2,16,23
  dw 0x0308+(reg1-16)*16+reg2-16
}

macro fmuls reg1,reg2
{
  _checkvalue 1,reg1,16,23
  _checkvalue 2,reg2,16,23
  dw 0x0380+(reg1-16)*16+reg2-16
}

macro fmulsu reg1,reg2
{
  _checkvalue 1,reg1,16,23
  _checkvalue 2,reg2,16,23
  dw 0x0388+(reg1-16)*16+reg2-16
}

macro icall
{
  dw 0x9509
}

macro ijmp
{
  dw 0x9409
}

macro in reg,port
{
  _checkvalue 1,port,0,63
  _checkvalue 2,reg,0,31
  dw 0xB000+(0x30 and port)*32+(0x0F and port)+reg*16
}

macro inc reg
{
  _checkvalue 1,reg,0,31
  dw 0x9403+reg*16
}

macro jmp address
{
  _bigjump address,0x940C
}

macro ld reg,arg1
{
  _checkvalue 1,reg,0,31

  local sign
  local srcreg
  local done

  done equ notyet

  match sr,arg1     \{
     srcreg equ sr
     sign equ none  \}
  match sr=+,arg1   \{
     srcreg equ sr
     sign equ plus  \}
  match =-sr,arg1   \{
     srcreg equ sr
     sign equ minus \}

  match =Z,srcreg
  \{
    match =plus ,sign  \\{ dw 0x9001+reg*16 \\}
    match =minus,sign  \\{ dw 0x9002+reg*16 \\}
    match =none ,sign  \\{ dw 0x8000+reg*16 \\}
    done equ
  \}
  match =Y,srcreg
  \{
    match =plus ,sign  \\{ dw 0x9009+reg*16 \\}
    match =minus,sign  \\{ dw 0x900A+reg*16 \\}
    match =none ,sign  \\{ dw 0x8008+reg*16 \\}
    done equ
  \}
  match =X,srcreg
  \{
    match =plus ,sign  \\{ dw 0x900D+reg*16 \\}
    match =minus,sign  \\{ dw 0x900E+reg*16 \\}
    match =none ,sign  \\{ dw 0x900C+reg*16 \\}
    done equ
  \}
  if done eq notyet
    display "want [-]X/Y/Z[+] instead '",`arg1,"'",13,10
    exit
  end if
}

macro ldd reg,arg1
{
  _checkvalue 1,reg,0,31

  local num
  local srcreg
  local done

  done equ notyet

  match sr,arg1     \{
     srcreg equ sr
     num equ 0  \}
  match sr=+n,arg1   \{
     srcreg equ sr
     num equ n  \}

  _checkvalue 2,num,0,63

  match =Z,srcreg
  \{
    dw 0x8000+reg*16+(0x20 and num)*256+(0x18 and num)*128+(0x07 and num)
    done equ
  \}
  match =Y,srcreg
  \{
    dw 0x8008+reg*16+(0x20 and num)*256+(0x18 and num)*128+(0x07 and num)
    done equ
  \}
  if done eq notyet
    display "want Y/Z[+num] instead '",`arg1,"'",13,10
    exit
  end if
}

macro ldi reg,val
{
  _checkvalue 1,reg,16,31
; _checkvalue 2,val,0,255
  dw 0xE000+(0xF0 and val)*16+(0x0F and val)+(0x0F and reg)*16
}

macro lds reg,address
{
  _checkvalue 1,reg,0,31
; _checkvalue 2,address,0,65535
  dw 0x9000+reg*16
  dw address
}

macro lpm reg,arg1
{
  if reg eq
    dw 0x95C8
  else
    _checkvalue 1,reg,0,31

    local sign
    local srcreg
    local done

    done equ notyet

    match sr,arg1     \{
      srcreg equ sr
      sign equ none  \}
    match sr=+,arg1   \{
      srcreg equ sr
      sign equ plus  \}

    match =Z,srcreg
    \{
      match =plus ,sign  \\{ dw 0x9005+reg*16 \\}
      match =none ,sign  \\{ dw 0x9004+reg*16 \\}
      done equ
    \}
    if done eq notyet
      display "want Z[+] instead '",`arg1,"'",13,10
      exit
    end if
  end if
}

macro lsl reg
{
  add reg,reg
}

macro lsr reg
{
  _checkvalue 1,reg,0,31
  dw 0x9406+reg*16
}

macro mov reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x2C00+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro movw reg1,reg2
{
  _checkvalue 1,reg1,0,31,1
  _checkvalue 2,reg2,0,31,1
  dw 0x0100+(reg1/2)*16+reg2/2
}

macro mul reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x9C00+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro muls reg1,reg2
{
  _checkvalue 1,reg1,16,31
  _checkvalue 2,reg2,16,31
  dw 0x0200+(reg1-16)*16+reg2-16
}

macro mulsu reg1,reg2
{
  _checkvalue 1,reg1,16,23
  _checkvalue 2,reg2,16,23
  dw 0x0300+(reg1-16)*16+reg2-16
}

macro neg reg
{
  _checkvalue 1,reg,0,31
  dw 0x9401+reg*16
}

macro nop
{
  dw 0
}

macro or reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x2800+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro ori reg,val
{
  _checkvalue 1,reg,16,31
  dw 0x6000+(0xF0 and val)*16+(0x0F and val)+(0x0F and reg)*16
}

macro out port,reg
{
  _checkvalue 1,port,0,63
  _checkvalue 2,reg,0,31
  dw 0xB800+(0x30 and port)*32+(0x0F and port)+reg*16
}

macro pop reg
{
  _checkvalue 1,reg,0,31
  dw 0x900F+reg*16
}

macro push reg
{
  _checkvalue 1,reg,0,31
  dw 0x920F+reg*16
}

macro rcall address
{
  _relativejump address,0xD000
}

macro ret
{
  dw 0x9508
}

macro reti
{
  dw 0x9518
}

macro rjmp address
{
  _relativejump address,0xC000
}

macro rol reg
{
  adc reg,reg
}

macro ror reg
{
  _checkvalue 1,reg,0,31
  dw 0x9407+reg*16
}

macro sbc reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x0800+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro sbci reg,val
{
  _checkvalue 1,reg,16,31
; _checkvalue 2,val,0,255
  dw 0x4000+(0x0F and reg)*16+(0xF0 and val)*16+(0x0F and val)
}

macro sbi port,bit
{
  _checkvalue 1,port,0,31
  _checkvalue 2,bit,0,7
  dw 0x9A00+port*8+bit
}

macro sbic port,bit
{
  _checkvalue 1,port,0,31
  _checkvalue 2,bit,0,7
  dw 0x9900+port*8+bit
}

macro sbis port,bit
{
  _checkvalue 1,port,0,31
  _checkvalue 2,bit,0,7
  dw 0x9B00+port*8+bit
}

macro sbiw reg,val
{
  _checkvalue 2,val,0,63

  if ~ reg in <r24,XL,YL,ZL>
    display "want r24,XL/YL/ZL instead '",`reg,"'",13,10
    error
  end if

  dw 0x9700+((reg-r24)/2*16)+(0x30 and val)*4+(0x0F and val)
}

macro sbr reg,val
{
  ori reg,val
}

macro sbrc reg,bit
{
  _checkvalue 1,reg,0,31
  _checkvalue 2,bit,0,7
  dw 0xFC00+reg*16+bit
}

macro sbrs reg,bit
{
  _checkvalue 1,reg,0,31
  _checkvalue 2,bit,0,7
  dw 0xFE00+reg*16+bit
}

macro sec { bset 0 }
macro seh { bset 5 }
macro sei { bset 2 }
macro sen { bset 0 }

macro ser reg
{
  ldi reg,0xFF
}

macro ses { bset 4 }
macro set { bset 6 }
macro sev { bset 3 }
macro sez { bset 1 }

macro sleep
{
  dw 0x9588
}

macro spm
{
  dw 0x95E8
}

macro st arg1,reg
{
  _checkvalue 2,reg,0,31

  local sign
  local srcreg
  local done

  done equ notyet

  match sr,arg1     \{
     srcreg equ sr
     sign equ none  \}
  match sr=+,arg1   \{
     srcreg equ sr
     sign equ plus  \}
  match =-sr,arg1   \{
     srcreg equ sr
     sign equ minus \}

  match =Z,srcreg
  \{
    match =plus ,sign  \\{ dw 0x9201+reg*16 \\}
    match =minus,sign  \\{ dw 0x9202+reg*16 \\}
    match =none ,sign  \\{ dw 0x8200+reg*16 \\}
    done equ
  \}
  match =Y,srcreg
  \{
    match =plus ,sign  \\{ dw 0x9209+reg*16 \\}
    match =minus,sign  \\{ dw 0x920A+reg*16 \\}
    match =none ,sign  \\{ dw 0x8208+reg*16 \\}
    done equ
  \}
  match =X,srcreg
  \{
    match =plus ,sign  \\{ dw 0x920D+reg*16 \\}
    match =minus,sign  \\{ dw 0x920E+reg*16 \\}
    match =none ,sign  \\{ dw 0x920C+reg*16 \\}
    done equ
  \}
  if done eq notyet
    display "want [-]X/Y/Z[+] instead '",`arg1,"'",13,10
    exit
  end if
}

macro std arg1,reg
{
  _checkvalue 2,reg,0,31

  local num
  local srcreg
  local done

  done equ notyet

  match sr,arg1     \{
     srcreg equ sr
     num equ 0  \}
  match sr=+n,arg1   \{
     srcreg equ sr
     num equ n  \}

  _checkvalue 1,num,0,63

  match =Z,srcreg
  \{
    dw 0x8200+reg*16+(0x20 and num)*256+(0x18 and num)*128+(0x07 and num)
    done equ
  \}
  match =Y,srcreg
  \{
    dw 0x8208+reg*16+(0x20 and num)*256+(0x18 and num)*128+(0x07 and num)
    done equ
  \}
  if done eq notyet
    display "want Y/Z[+num] instead '",`arg1,"'",13,10
    exit
  end if
}

macro sts address,reg
{
; _checkvalue 1,address,0,65535
  _checkvalue 2,reg,0,31
  dw 0x9200+reg*16
  dw address
}

macro sub reg1,reg2
{
  _checkvalue 1,reg1,0,31
  _checkvalue 2,reg2,0,31
  dw 0x1800+(0x10 and reg2)*32+(0x0F and reg2)+(0x1F and reg1)*16
}

macro subi reg,val
{
  _checkvalue 1,reg,16,31
; _checkvalue 2,val,0,255
  dw 0x5000+(0xF0 and val)*16+(0x0F and val)+(0x0F and reg)*16
}

macro swap reg
{
  _checkvalue 1,reg,0,31
  dw 0x9402+reg*16
}

macro tst reg
{
  and reg,reg
}

macro wdr
{
  dw 0x95A8
}
