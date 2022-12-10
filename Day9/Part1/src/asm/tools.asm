    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getNumber
    public _getChar
    public _putTail

offsetPointer := ti.appData
offset := offsetPointer + 3

displayWidth := 370

_getNumber:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset in appvar to begin searching
    pop ix
    ld (offsetPointer), hl
    ld hl, (hl)
    ld (offset), hl
    ld hl, inputVar
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    ret c ; no input file found
    call ti.ChkInRam
    jr z, .inRam
    ld hl, 10
    add hl, de
    ld a, c
    ld bc, 0
    ld c, a
    add hl, bc
    ex de, hl

.inRam:
    inc de
    inc de ; skip size bytes
    ld hl, (offset)
    add hl, de ; skip offset
    push hl ; pointer to first line
    call ti.StrLength
    ld hl, (offset)
    add hl, bc ; update offset
    inc hl
    ld de, (offsetPointer)
    ex de, hl
    ld (hl), de
    pop hl
    ex de, hl

.loopFind: ; find where the number begins
    ld a, (de)
    cp a, ' '
    inc de
    jr nz, .loopFind
    ld bc, 0
    push bc

.loopNumber:
    ld a, (de)
    sub a, $30
    ld hl, 0
    ld l, a
    pop bc
    add hl, bc
    inc de
    ld a, (de)
    cp a, '0'
    ret c
    cp a, ':'
    ret nc
    push hl
    pop bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl
    push hl
    jr .loopNumber

_getChar:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset in appvar to begin searching
    pop ix
    push hl
    ld hl, inputVar
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    ret c ; no input file found
    call ti.ChkInRam
    jr z, .inRam
    ld hl, 10
    add hl, de
    ld a, c
    ld bc, 0
    ld c, a
    add hl, bc
    ex de, hl

.inRam:
    inc de
    inc de ; skip size bytes
    pop hl
    add hl, de
    ld a, (hl)
    ret

_putTail:
    push ix
    ld ix, 0
    add ix, sp
    ld bc, (ix + 6) ; x
    ld de, (ix + 9) ; y
    pop ix
    ld hl, $D40000
    add hl, bc
    push de
    pop bc
    ld de, displayWidth
    call _addYVal
    ld a, $ff
    cp a, (hl)
    ld (hl), 0
    ld a, 1
    ret z
    dec a
    ret

_addYVal: ; really slow and bad optimization
    call ti.ChkBCIs0
    ret z
    add hl, de
    dec bc
    jr _addYVal

inputVar:
    db ti.AppVarObj, 'Input', 0
