static  void mem_put_le32(void *vmem, int val) {
  unsigned char *mem = (unsigned char *)vmem;

  mem[0] = (val >>  0) & 0xff;
  mem[1] = (val >>  8) & 0xff;
  mem[2] = (val >> 16) & 0xff;
  mem[3] = (val >> 24) & 0xff;
}

static  void mem_put_le16(void *vmem, int val) {
  unsigned char  *mem = (unsigned char  *)vmem;

  mem[0] = (val >>  0) & 0xff;
  mem[1] = (val >>  8) & 0xff;
}


void HardwareVideoCombo::WriteVP8(unsigned char* buf, uint32_t frameIdx, uint32_t len, int64_t cs ) {
	if(frameIdx == 0) { // write IVF file header first
		LOG(LS_INFO) << "----------in frameIdx == 0";
		char ivfFileHdr[32];
		uint16_t ivfFileHdrLen = 32;
		uint16_t width = 640;
		uint16_t height = 480;
		uint32_t framerate = 30;
		uint32_t timescale = 1;
	
		ivfFileHdr[0] = 'D';
		ivfFileHdr[1] = 'K';
		ivfFileHdr[2] = 'I';
		ivfFileHdr[3] = 'F';
		ivfFileHdr[4] = '\0';
		ivfFileHdr[5] = '\0';
		mem_put_le16(ivfFileHdr + 6, ivfFileHdrLen);
		ivfFileHdr[8] = 'V';
		ivfFileHdr[9] = 'P';
		ivfFileHdr[10] = '8';
		ivfFileHdr[11] = '0';
		
		mem_put_le16(ivfFileHdr + 12, width);
		mem_put_le16(ivfFileHdr + 14, height);
		mem_put_le32(ivfFileHdr + 16, framerate);
		mem_put_le32(ivfFileHdr + 20, timescale);
		mem_put_le32(ivfFileHdr + 24, 300); 	/* length */ // note, here you should know how many frames(we just record 300 frames in this test) is recorded in this .ivf file before you start record.
		fwrite(ivfFileHdr, sizeof(ivfFileHdr), 1, pFile_);
	  }
	
	  char ivfFrameHdr[12];
	  mem_put_le32(ivfFrameHdr, len);  
	  mem_put_le32(ivfFrameHdr+ 4, (int)(cs & 0xFFFFFFFF));
	  mem_put_le32(ivfFrameHdr + 8, (int)(cs >> 32));
	  fwrite(ivfFrameHdr, sizeof(ivfFrameHdr), 1, pFile_);
	
	  fwrite(buf, len, 1, pFile_);
	
	  if (frameIdx == 300) {
		 LOG(LS_INFO) << "----------in frameIdx == 300";
		  fclose(pFile_);
	  }

}


void main() {

  WriteVP8(bufferVP8, frame_idx, bufferLen, (int64_t)frame_idx*FRAMERATE);

}
