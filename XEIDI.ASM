; xeidi.asm - _sys_xdisabl, _sys_xrestor, _sys_pcxget, _sys_getstk

	include	dos.asm		; segment macros

	dseg
; null data segment
	endds

	pseg

	public	_sys_xdisabl, _sys_xrestor, _sys_pcxget, _sys_getstk
	extrn	pcxflag:word
	extrn	sssave:word
	extrn	spsave:word

;-------------------------------------------------------------------------
; _sys_xdisabl  --  return pcxflag & disable context switching
;-------------------------------------------------------------------------
; int sys_xdisabl()
_sys_xdisabl	proc	near
	pushf
	cli			; disable interrupts
	xor	ax,ax
	xchg	ax,cs:pcxflag
	popf
	ret
_sys_xdisabl	endp

;-------------------------------------------------------------------------
; _sys_xrestor  --  restore pcxflag
;-------------------------------------------------------------------------
; sys_xrestor(ps)
; int ps;
_sys_xrestor	proc	near
	push	bp
	mov	bp,sp		; C calling convention
	pushf
	cli			; disable interrupts
	mov	ax,[bp+4];	; get passed flags word
	mov	cs:pcxflag,ax	; reset pcxflag to passed value
	popf
	pop	bp
	ret
_sys_xrestor	endp

;-------------------------------------------------------------------------
; _sys_pcxget  --  get the current value of pcxflag
;-------------------------------------------------------------------------
; int sys_pcxget()
_sys_pcxget	proc	near
	pushf
	cli			; disable interrupts
	mov	ax,cs:pcxflag
	popf
	ret
_sys_pcxget	endp

;-------------------------------------------------------------------------
; sys_getstk  --  get the stack parameters for panic printing
;-------------------------------------------------------------------------
; sys_getstk(sssp,spsp)
; int *sssp,*spsp
_sys_getstk	proc	near
	ASSUME DS:DGROUP
	push	bp
	mov	bp,sp
	mov	bx,[bp+4]
	mov	ax,cs:sssave
	mov	[bx],ax
	mov	bx,[bp+6]
	mov	ax,cs:spsave
	mov	[bx],ax
	pop	bp
	ret
_sys_getstk	endp

	endps

	end
