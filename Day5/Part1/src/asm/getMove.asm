    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getMove

offsetPointer := ti.appData
offset := offsetPointer + 3
arrayPointer := offset + 3

_getMove: ; get the start of instructions
    push ix
    pop de
    ld ix, 0
    add ix, sp
    ld hl, (ix + 3) ; offset
    ld bc, (ix + 12) ; where to move
    push bc
    ld bc, (ix + 9) ; move from
    push bc
    ld bc, (ix + 6) ; how many to move
    push bc
    push de
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
    push hl ; pointer to first instruction
    call ti.StrLength
    ld hl, (offset)
    add hl, bc ; update offset
    inc hl
    ld de, (offsetPointer)
    ex de, hl
    ld (hl), de
    pop de ; pointer to first instruction
    ld hl, 5
    add hl, de ; skip "move"
    ex de, hl
    call _loadNumber
    ld hl, 6
    add hl, de
    ex de, hl
    call _loadNumber
    ld hl, 4
    add hl, de
    ex de, hl
    call _loadNumber
    ret

_loadNumber:
    ld bc, 0
    push bc

.loop:
    ld a, (de)
    sub a, $30 ; get an actual number
    ld c, a
    pop hl
    add hl, bc
    inc de
    ld a, (de)
    cp a, '!' ; check for the next word/end
    jr c, .return
    push hl
    pop bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl
    push hl
    jr .loop

.return:
    push hl
    pop bc
    pop iy
    pop hl ; pointer to thing to load
    push iy ; return pointer
    ld (hl), bc
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0

