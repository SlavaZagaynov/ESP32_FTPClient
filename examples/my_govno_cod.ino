
  
//---------------------------------------------------------
  void getcfg(){
	  
	  
/* 	  
File file = SPIFFS.open("/dbg.txt", FILE_READ);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  } */

 ftp.OpenConnection();
  ftp.ChangeWorkDir("/hp500/esp/");


   // Get directory content
  ftp.InitFile("Type A");
  String list[128];
  String list_all;
  //ftp.ChangeWorkDir("/hp500/esp/");
  ftp.ContentListWithListCommand("", list);  
  //ftp.ContentList("", list);  
  //Serial.println("\nDirectory info: ");
  for (int i = 0; i < sizeof(list); i++) {
    if(list[i].length() > 0){
		//Serial.println(list[i]);
		list_all += list[i]+"\n";}
    else{
		break;
		}
  } 

//Serial.println(list_all);

if (list_all.indexOf(IP)<0){
    // Make a new directory
   ftp.InitFile("Type A");
   ftp.MakeDir(IP.c_str());
}

   ftp.ChangeWorkDir(IP.c_str()); 	

  readAndSendBigBinFile(SPIFFS, "dbg.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "cfg.json", ftp); 
  readAndSendBigBinFile(SPIFFS, "a1.json", ftp); 
  readAndSendBigBinFile(SPIFFS, "tft.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "rf.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "ir.txt", ftp); 

//или 
/* 
  File file = SPIFFS.open("/ESP32-WROOM-32_BANNER_1_0.jpg");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  // Create the new file and send the image
  ftp.InitFile("Type I");
  ftp.NewFile("ESP32-WROOM.jpg");
  //Becarefull file reference
  ftp.WriteData(&file);
  ftp.CloseFile(); */
  


  ftp.CloseConnection();

return;
  }





// ReadFile Example from ESP32 SD_MMC Library within Core\Libraries
// Changed to also write the output to an FTP Stream
void readAndSendBigBinFile(fs::FS& fs, const char* path, ESP32_FTPClient ftpClient) {
    
    String fullPath = "/";
    fullPath.concat(path);
    //Serial.printf("Reading file: %s\n", fullPath);

    File file = fs.open(fullPath);
    if (!file) {        Serial.println("Failed to open file for reading");
        return;
    }

	Serial.println("Read from file: "+String(file.name())+" SIZE: "+(String(file.size() )));
	//ftpClient.WriteData(file.read());

    ftpClient.InitFile("Type I");
    ftpClient.NewFile(path);

	int fsize = file.size();	
	int rb = 1024; 	
	unsigned char buf[rb];
		
    while (file.available()) {
		if (fsize <= 1024){	rb = fsize; }
	    //Serial.println(rb);
        int readVal = file.read(buf, rb);
        ftpClient.WriteData(buf, rb);
		fsize = fsize - rb;
    }
	
    ftpClient.CloseFile();
    file.close();
}

