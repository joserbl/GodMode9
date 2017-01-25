#include "sddata.h"
#include "virtual.h"
#include "ffconf.h"

#define VFIL(fp) ((VirtualFile*) (void*) fp->buf)

FRESULT fvx_open (FIL* fp, const TCHAR* path, BYTE mode) {
    #if !_FS_TINY
    VirtualFile* vfile = VFIL(fp);
    memset(fp, 0, sizeof(FIL));
    if (GetVirtualFile(vfile, path)) {
        fp->obj.fs = NULL;
        fp->obj.objsize = vfile->size;
        fp->fptr = 0;
        return FR_OK;
    }
    #endif
    return fx_open ( fp, path, mode );
}

FRESULT fvx_read (FIL* fp, void* buff, UINT btr, UINT* br) {
    #if !_FS_TINY
    if (fp->obj.fs == NULL) {
        VirtualFile* vfile = VFIL(fp);
        int res = ReadVirtualFile(vfile, buff, fp->fptr, btr, (u32*) br);
        fp->fptr += *br;
        return res;
    }
    #endif
    return fx_read ( fp, buff, btr, br );
}

FRESULT fvx_write (FIL* fp, const void* buff, UINT btw, UINT* bw) {
    #if !_FS_TINY
    if (fp->obj.fs == NULL) {
        VirtualFile* vfile = VFIL(fp);
        int res = WriteVirtualFile(vfile, buff, fp->fptr, btw, (u32*) bw);
        fp->fptr += *bw;
        return res;
    }
    #endif
    return fx_write ( fp, buff, btw, bw );
}

FRESULT fvx_close (FIL* fp) {
    #if !_FS_TINY
    if (fp->obj.fs == NULL) return FR_OK;
    #endif
    return fx_close( fp );
}

FRESULT fvx_lseek (FIL* fp, FSIZE_t ofs) {
    #if !_FS_TINY
    if (fp->obj.fs == NULL) {
        fp->fptr = ofs;
        return FR_OK;
    }
    #endif
    return f_lseek( fp, ofs );
}

FRESULT fvx_sync (FIL* fp) {
    #if !_FS_TINY
    if (fp->obj.fs == NULL) return FR_OK;
    #endif
    return f_sync( fp );
}