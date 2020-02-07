#ifndef _EXT2_H_
#define _EXT2_H_

#include "common.h"
#include "disk.h"
#include "types.h"

#define EXT2_NAME_LEN 11

#define EXT2_BAD_INO 1
#define EXT2_ROOT_INO 2
#define EXT2_BOOT_LOADER_INO 5
#define EXT2_UNDEL_DIR_INO 6
#define EXT2_GOOD_OLD_FIRST_INO 11

#define EXT2_FT_UNKNOWN 0
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR 2
#define EXT2_FT_CHRDEV 3
#define EXT2_FT_BLKDEV 4
#define EXT2_FT_FIFO 5
#define EXT2_FT_SOCK 6
#define EXT2_FT_SYMLINK 7

typedef struct {
    UINT16 mode;
    UINT16 uid;
    UINT32 size;
    UINT32 atime; // 마지막 접근
    UINT32 ctime; // 마지막 변경(inode)
    UINT32 mtime; // 마지막 변경(파일 내용)
    UINT32 dtime; // 파일이 삭제된 시간
    UINT16 gid;
    UINT16 link_cnt;
    UINT32 block_cnt;
    UINT32 flags;
    BYTE OS_description_1[4]; // 운영체제 정보
    UINT32 i_block[15]; // data block, 0~11 : block, 12 : 단일 간접, 13 : 이중 간접, 14 : 삼중 간접
    UINT32 generation; // Network FS를 위한 파일 버전
    UINT32 file_acl;
    UINT32 dir_acl;
    UINT32 faddr; // 단편 주소
    BYTE OS_DESCRIPTION_2[12];
} INODE; // 128 byte

typedef struct {
    UINT32 inode;
    UINT16 record_len; // 현재 디렉토리 엔트리 크기
    BYTE name_len; // 문자열 길이를 저장하기 때문에 name의 마지막에 null을 채울 필요 없음?
    BYTE file_type;
    BYTE name[EXT2_NAME_LEN];
} EXT2_DIR_ENTRY;

typedef struct {
    UINT32 group;
    UINT32 block;
    UINT32 offset;
} EXT2_DIR_ENTRY_LOCATION;

typedef struct {
    UINT32 max_inode_count;				//0x00
    UINT32 block_count;
    UINT32 reserved_block_count;
    UINT32 free_block_count;		
    UINT32 free_inode_count;			//0x10
    UINT32 first_data_block; // partition에서 boot block이 끝나고 block group이 시작하는 위치
    UINT32 log_block_size; // block size의 2의 지수
    UINT32 log_fragmentation_size;		
    UINT32 block_per_group;				//0x20
    UINT32 fragmentation_per_group;
    UINT32 inode_per_group;
	UINT32 mtime;		
	UINT32 wtime;						//0x30
	UINT16 mount_cnt;
	UINT16 max_mount_cnt;
    UINT16 magic_signature;
    UINT16 state;
    UINT16 errors;
	UINT16 minor_version;
	UINT32 last_check;					//0x40
	UINT32 check_interval;
	UINT32 creator_OS;
	UINT32 major_version;
	UINT16 def_res_uid;					//0x50
	UINT16 def_res_gid;
	UINT32 first_ino;
	UINT16 inode_size;
	UINT16 block_group_num;
	UINT32 feature_compat; 
	UINT32 feature_incompat; 			//0x60
	UINT32 feature_read_only_compat;
	BYTE uuid[16];
	BYTE volume_name[16];
	BYTE last_mounted[64];
	UINT32 algorithm_usage_bitmap;
	BYTE prealloc_block;
	BYTE prealloc_dir_block;
	UINT16 padding_1;
	BYTE journal_uuid[16];				//0xD0
	UINT32 journal_inode_num;
	UINT32 journal_dev;
	UINT32 last_orphan;
	BYTE hash_seed[16];
	BYTE def_hash_version;
	BYTE padding_2;
	UINT16 padding_3;
	UINT32 default_mount_opt; 			//0x100
	UINT32 first_meta_bg;

    //UINT32 first_non_reserved_inode;
    //UINT16 inode_structure_size;
	//UINT16 block_group_number;
} EXT2_SUPER_BLOCK;

typedef struct {
    UINT32 start_block_of_block_bitmap;
    UINT32 start_block_of_inode_bitmap;
    UINT32 start_block_of_inode_table;
    UINT16 free_blocks_count;
    UINT16 free_inodes_count;
    UINT16 directories_count; // Block Group 내에 생설된 디레곹리 수
    BYTE padding[2];
    BYTE reserved[12];
} EXT2_GRUOP_DESCRIPTOR;

typedef struct {
    EXT2_SUPER_BLOCK sb;
    EXT2_GRUOP_DESCRIPTOR gd;
    DISK_OPERATIONS *disk;
} EXT2_FILESYSTEM;

typedef struct {
    EXT2_FILESYSTEM *fs;
    EXT2_DIR_ENTRY entry;
    EXT2_DIR_ENTRY_LOCATION location;
} EXT2_NODE;

#endif // _EXT2_H_