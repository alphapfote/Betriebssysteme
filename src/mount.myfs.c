//
//  mount.myfs.c
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "wrap.h"

#include <fuse.h>
#include <stdio.h>

#include "myfs-info.h"

//"fuse", ein framework das uns die kernelprogrammierung abnimmt, wird eingebunden
struct fuse_operations myfs_oper;

int main(int argc, char *argv[]) {
    int fuse_stat;
    
    //den funktionen von fuse werden funktionen aus unserem wrapper zugeteilt
    myfs_oper.getattr = wrap_getattr; //metadaten der datei lesen
    myfs_oper.readlink = wrap_readlink;
    myfs_oper.getdir = NULL;
    myfs_oper.mknod = wrap_mknod; //erzeugt eine datei
    myfs_oper.mkdir = wrap_mkdir; // verzeichnis erzeugen
    myfs_oper.unlink = wrap_unlink; //löscht eine datei
    myfs_oper.rmdir = wrap_rmdir; //verzeichnis löschen
    myfs_oper.symlink = wrap_symlink;
    myfs_oper.rename = wrap_rename; //datei verschieben oder umgenennen
    myfs_oper.link = wrap_link;
    myfs_oper.chmod = wrap_chmod; //zugriffsrechte setzen
    myfs_oper.chown = wrap_chown; //besitzer ändern
    myfs_oper.truncate = wrap_truncate;//datei an stelle "len" abschneiden
    myfs_oper.utime = wrap_utime;
    myfs_oper.open = wrap_open;//datei öffnen
    myfs_oper.read = wrap_read; //daten aus datei lesen
    myfs_oper.write = wrap_write; //daten in datei schreiben
    myfs_oper.statfs = wrap_statfs;
    myfs_oper.flush = wrap_flush;//datei zurückschreiben?
    myfs_oper.release = wrap_release;//datei entgüötig schließen
    myfs_oper.fsync = wrap_fsync;
    myfs_oper.setxattr = wrap_setxattr;
    myfs_oper.getxattr = wrap_getxattr;
    myfs_oper.listxattr = wrap_listxattr;
    myfs_oper.removexattr = wrap_removexattr;
    myfs_oper.opendir = wrap_opendir;
    myfs_oper.readdir = wrap_readdir; //liefert verzeichniseinträge
    myfs_oper.releasedir = wrap_releasedir;
    myfs_oper.fsyncdir = wrap_fsyncdir;
    myfs_oper.init = wrap_init;
    
    // FsInfo will be used to pass information to fuse functions
    struct MyFsInfo *FsInfo;
    FsInfo= malloc(sizeof(struct MyFsInfo));
    
    // parse arguments
    if(argc > 3) {
        fprintf(stderr, "Containerfile= %s\n", argv[1]);
        fprintf(stderr, "Logfile=       %s\n", argv[2]);
        fprintf(stderr, "Mountpoint=    %s\n", argv[3]);

        // container & log file name will be passed to fuse functions
        FsInfo->contFile= argv[1];
        FsInfo->logFile= argv[2];
                
        // adjust arguments
        argv+= 2; argc-= 2;
    }
    else {
        fprintf(stderr, "Usage: %s containerfile logfile mountpoint\n", argv[0]);
        return (EXIT_FAILURE);
    }
    
    fuse_stat = fuse_main(argc, argv, &myfs_oper, FsInfo);
    
    fprintf(stderr, "fuse_main returned %d\n", fuse_stat);
    
    // cleanup
    free(FsInfo);
    
    return fuse_stat;
}


