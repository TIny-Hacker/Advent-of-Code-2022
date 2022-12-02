    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getLine

_getLine:
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
    push hl
    ld hl, (hl)
    push hl
    add hl, de ; skip offset
    ex de, hl
    pop hl
    ld bc, 4
    add hl, bc
    push hl
    pop bc
    pop hl ; pointer to offset
    ld (hl), bc
    ex de, hl ; put pointer to char * in hl
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
