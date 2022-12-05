    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getStart

offsetPointer := ti.appData
offset := offsetPointer + 3
arrayPointer := offset + 3

_getStart: ; get the start of instructions
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset
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

.loop:
    ld a, (de)
    cp a, 'm' ; all instrutions start with an "m", so we know this is the first one
    ret z
    inc de
    ld bc, (offset)
    inc bc
    ld (offset), bc
    ld hl, (offsetPointer)
    ld (hl), bc
    jr .loop

inputVar:
    db ti.AppVarObj, 'Input', 0
