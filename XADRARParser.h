/*
 * XADRARParser.h
 *
 * Copyright (c) 2017-present, MacPaw Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */
#import "XADArchiveParser.h"
#import "CSInputBuffer.h"

typedef struct RARBlock
{
	int crc,type,flags;
	int headersize;
	off_t datasize;
	off_t start,datastart;
	CSHandle *fh;
} RARBlock;

typedef struct RARFileHeader
{
	off_t size;
	int os;
	uint32_t crc,dostime;
	int version,method,namelength;
	uint32_t attrs;
	NSData *namedata,*salt;
} RARFileHeader;

@interface XADRARParser:XADArchiveParser
{
	int archiveflags,encryptversion;

//	NSMutableDictionary *lastcompressed;
	NSMutableDictionary *keys;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;
+(NSArray *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)setPassword:(NSString *)newpassword;

-(void)parse;

-(RARFileHeader)readFileHeaderWithBlock:(RARBlock *)block;
-(NSData *)readComment;

-(RARBlock)readBlockHeader;
-(void)skipBlock:(RARBlock)block;

-(void)addEntryWithBlock:(const RARBlock *)block header:(const RARFileHeader *)header
compressedSize:(off_t)compsize files:(NSArray *)files solidOffset:(off_t)solidoffs
isCorrupted:(BOOL)iscorrupted;
-(XADPath *)parseNameData:(NSData *)data flags:(int)flags;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;

-(CSInputBuffer *)inputBufferForFileWithIndex:(int)file files:(NSArray *)files;
-(CSHandle *)inputHandleForFileWithIndex:(int)file files:(NSArray *)files;
-(CSHandle *)inputHandleWithParts:(NSArray *)parts encrypted:(BOOL)encrypted
cryptoVersion:(int)version salt:(NSData *)salt;
-(NSData *)keyForSalt:(NSData *)salt;

-(off_t)outputLengthOfFileWithIndex:(int)file files:(NSArray *)files;

-(NSString *)formatName;

@end


@interface XADEmbeddedRARParser:XADRARParser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;

-(void)parse;
-(NSString *)formatName;

@end
