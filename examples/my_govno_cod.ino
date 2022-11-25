//---------------------------------------------------------
int getcfg() {

  ftp.OpenConnection();
  if (!ftp.isConnected()) { return 0; }

  ftp.ChangeWorkDir("/hp500/esp/");

  // Get directory content
  ftp.InitFile("Type A");
  String list[128];
  String list_all;
  ftp.ContentListWithListCommand("", list);
  //ftp.ContentList("", list);
  //Serial.println("\nDirectory info: ");
  for (int i = 0; i < sizeof(list); i++) {
    if (list[i].length() > 0) {
      //Serial.println(list[i]);
      list_all += list[i] + "\n";
    } else {
      break;
    }
  }

  //Serial.println(list_all);

  if (list_all.indexOf(IP) < 0) {
    // Make a new directory
    ftp.InitFile("Type A");
    ftp.MakeDir(IP.c_str());
  }
  ftp.ChangeWorkDir(IP.c_str());


  Serial.println("Listing directory:");
  File root = SPIFFS.open("/");

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {

    } else {
      /*             Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size()); */

      readAndSendBigBinFile(SPIFFS, file.name(), ftp);
    }
    file = root.openNextFile();
  }


  /*   
  readAndSendBigBinFile(SPIFFS, "dbg.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "cfg.json", ftp); 
  readAndSendBigBinFile(SPIFFS, "a1.json", ftp); 
  readAndSendBigBinFile(SPIFFS, "tft.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "rf.txt", ftp); 
  readAndSendBigBinFile(SPIFFS, "ir.txt", ftp);  
  */

  

  ftp.CloseConnection();

  return 1;
}




//---------------------------------------------------------
// ReadFile Example from ESP32 SD_MMC Library within Core\Libraries
// Changed to also write the output to an FTP Stream
void readAndSendBigBinFile(fs::FS& fs, const char* path, ESP32_FTPClient ftpClient) {

  String fullPath = "/";
  fullPath.concat(path);
  //Serial.printf("Reading file: %s\n", fullPath);

  File file = fs.open(fullPath);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: " + String(file.name()) + " SIZE: " + (String(file.size())));

  ftpClient.InitFile("Type I");
  ftpClient.NewFile(path);

  int fsize = file.size();
  int rb = 1024;
  unsigned char buf[1024];

  while (file.available()) {

    if (fsize <= 1024) { rb = fsize; }
    Serial.print(".");
    int readVal = file.read(buf, rb);
    ftpClient.WriteData(buf, rb);
    fsize = fsize - rb;
  }

  Serial.println(".");

  ftpClient.CloseFile();
  file.close();
}
