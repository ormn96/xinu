; stack.asm - _sys_stknpb, _sys_stkinit

	include	dos.asm

	dseg
; MS-DOS C compiler stack limit register
	extrn	STKHQQ:word
	endds

	pseg

	public	_sys_stknpb
	public	_sys_stkinit

;-------------------------------------------------------------------------
; _sys_stknpb  --  return null process pbase stack base pointer
;-------------------------------------------------------------------------
; char *sys_stknpb()
_sys_stknpb	proc	near
	mov	ax,STKHQQ		; get lower stack limit
	ret
_sys_stknpb	endp

;-------------------------------------------------------------------------
; _sys_stkinit  --  run-time stack initialization
;-------------------------------------------------------------------------
; void sys_stkinit()
_sys_stkinit	proc	near
	mov	STKHQQ,0		; set stack base to zero
	ret
_sys_stkinit	endp

	endps

	end
