
#include "headers/mn_gfx.h"

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#define	FIFO_SIZE (256*1024)
void *fifoBuffer;
static void	*xfb;
GXRModeObj* screenMode;

void update_screen(Mtx viewMatrix);



//this function+variable pair isnt useful really because
//wether or not to render should be determined by the
//"Hollywood"'s availability rather than whatever this is
static void	next_frame(u32 unused);
static bool frame_just_happened;



Mtx	view;
Mtx44	projection;
GXColor	backgroundColor	= {0xff, 0x00, 0x33, 0xff};
//all of the matrix math below is redundant for a 2D game
// and may be simplified later to make the code simpler
guVector camera =	{0.0F, 0.0F, 0.0F};
guVector up =	{0.0F, 1.0F, 0.0F};
guVector look	= {0.0F, 0.0F, -1.0F};



void mn_gfx_init() {
	VIDEO_Init();
  screenMode = &TVNtsc240Ds;

	//the xfb is the image that is directly seen on the television 
	xfb	= MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));

	
	VIDEO_Configure(screenMode);
	VIDEO_SetNextFramebuffer(xfb);
	
	VIDEO_SetPostRetraceCallback(next_frame);

	//I don't know what this is but it seems important
	VIDEO_SetBlack(false);

	//Yeah I don't know
	VIDEO_Flush();

	//thats where the gpu commands go
	fifoBuffer = MEM_K0_TO_K1(memalign(32,FIFO_SIZE));

	memset(fifoBuffer,	0, FIFO_SIZE);
	
	//that initializes gx which is going to like use that fifo buffer
	GX_Init(fifoBuffer, FIFO_SIZE);
	
	//??
	GX_SetCopyClear(backgroundColor, 0x00ffffff);
	
	//okay so this uh does something
	GX_SetViewport(0,0,screenMode->fbWidth,screenMode->efbHeight,0,1);
	GX_SetDispCopyYScale((f32)screenMode->xfbHeight/(f32)screenMode->efbHeight);
	GX_SetScissor(0,0,screenMode->fbWidth,screenMode->efbHeight);
	GX_SetDispCopySrc(0,0,screenMode->fbWidth,screenMode->efbHeight);
	GX_SetDispCopyDst(screenMode->fbWidth,screenMode->xfbHeight);
	GX_SetCopyFilter(screenMode->aa,screenMode->sample_pattern,
					 GX_TRUE,screenMode->vfilter);
	GX_SetFieldMode(screenMode->field_rendering,
					((screenMode->viHeight==2*screenMode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	
	//yeah
	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(xfb,GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);
	
	
	guPerspective(
		projection,
		60,
			(CONF_GetAspectRatio() == CONF_ASPECT_16_9) ?
			16.0F/9.0F : 4.0F/3.0F,
		.1F,
		300.0F
	);
	GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX8);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,	GX_POS_XYZ,	GX_S16,	0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8,	0);
	GX_SetNumChans(1);
	GX_SetNumTexGens(0);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	//GX_SetDither(1);
}

void mn_gfx_vertex_pointers(void* pos_ptr, void* col_ptr) {
	GX_SetArray(GX_VA_POS, pos_ptr, 3*sizeof(s16));
	GX_SetArray(GX_VA_CLR0,	col_ptr,	4*sizeof(u8));	
}


void mn_gfx_update() {
	guLookAt(view, &camera, &up, &look);
	GX_SetViewport(0,0,screenMode->fbWidth,screenMode->efbHeight,0,1);
	GX_InvVtxCache();
	GX_InvalidateTexAll();
	Mtx	modelView;

	guMtxIdentity(modelView);
	guMtxTransApply(modelView, modelView, 0.0F,	0.0F, -50.0F);
	guMtxConcat(view,modelView,modelView);

	GX_LoadPosMtxImm(modelView,	GX_PNMTX0);

	GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);

	GX_Position1x8(0);
	GX_Color1x8(0);
	GX_Position1x8(1);
	GX_Color1x8(1);
	GX_Position1x8(2);
	GX_Color1x8(2);
	GX_End();
	GX_DrawDone();
	GX_CopyDisp(xfb,GX_TRUE);
	return;
} //vsync more like peesync.
// the meaning of this statement is that that pee-sync is a more descriptive word than vsync because its urine-esque uselessness is a more prominent
// and name-worthy characteristic of the practice of only updating the EFB during the brief time interval when the television goes dark than the reason why
// it is going dark that being that once the beam returns it will be far vertically higher hence the V. The reason why it is deemed to be so useless to the
// point that it is recommended to have a name that alludes to its uselessness is because using vsync limits the response time.
uint8_t mn_gfx_is_ready() {
	return frame_just_happened;
}

static void	next_frame(u32 count __attribute__ ((unused))) {
	frame_just_happened = true;
}