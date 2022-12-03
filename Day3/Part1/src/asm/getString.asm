    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getString

offsetPointer := ti.appData
offset := offsetPointer + 3

_getString:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset in appvar to begin searching
    ld bc, (ix + 9) ; pointer to length
    pop ix
    push bc
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
    push hl ; pointer to string
    call ti.StrLength
    ld hl, (offset)
    add hl, bc ; update offset
    inc hl
    ld de, (offsetPointer)
    ex de, hl
    ld (hl), de
    pop de ; string
    pop hl ; length
    ld (hl), bc
    ex de, hl ; return string
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
