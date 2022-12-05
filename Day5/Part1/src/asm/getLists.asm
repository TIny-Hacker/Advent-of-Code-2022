    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getLists

offsetPointer := ti.appData
offset := offsetPointer + 3
arrayPointer := offset + 3

_getLists:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset
    ld bc, (ix + 9) ; pointer to array to load
    pop ix
    ld (arrayPointer), bc
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
    ex de, hl

.loop:
    ld a, (de)
    cp a, 'A'
    jr c, .notLetter ; It's a space or number
    ld hl, (arrayPointer)
    ld (hl), a
    inc hl
    ld (arrayPointer), hl

.notLetter:
    cp a, '1' ; number?
    jr nc, .maybeReturn

.next:
    ld hl, 36
    add hl, de
    ex de, hl ; next row
    jr .loop

.maybeReturn:
    cp a, ':' ; is it a letter?
    jr nc, .next

.return:
    xor a, a
    ld hl, (arrayPointer)
    ld (hl), a
    ld (arrayPointer), hl ; zero terminate
    ld de, (offset)
    inc de
    inc de
    inc de
    inc de ; next column
    ld hl, (offsetPointer)
    ld (hl), de
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
