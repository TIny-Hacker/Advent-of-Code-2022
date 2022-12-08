    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getLine
    public _getNumber

offsetPointer := ti.appData
offset := offsetPointer + 3

_getLine:
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
    ret

_getNumber:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; pointer to start of string
    pop ix
    ex de, hl
    ld bc, 0
    push bc

.loop:
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
    push hl
    pop bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl
    push hl
    jr .loop

inputVar:
    db ti.AppVarObj, 'Input', 0
