function doGet(e) {
  var cardId = e.parameter.cardId;
  
  if (!cardId) {
    return ContentService.createTextOutput("Error: Missing Card ID");
  }
  
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var rosterSheet = ss.getSheetByName("Roster");
  var attendanceSheet = ss.getSheetByName("Attendance");
  
  if (!rosterSheet || !attendanceSheet) {
    return ContentService.createTextOutput("Error: Sheet tabs not named correctly");
  }
  
  // 1. DYNAMICALLY SEARCH THE ROSTER
  var rosterData = rosterSheet.getDataRange().getValues();
  var studentName = "Unknown Student"; 
  
  // Loops through every entry from row 2 down to the absolute end of the sheet data
  for (var i = 1; i < rosterData.length; i++) {
    if (rosterData[i][0].toString().trim().toUpperCase() === cardId.toString().trim().toUpperCase()) {
      studentName = rosterData[i][1]; 
      break; 
    }
  }
  
  // 2. LOG THE DETECTED ATTENDANCE SCAN
  var timestamp = new Date();
  attendanceSheet.appendRow([timestamp, cardId, studentName]);
  
  // 3. RESPOND DIRECTLY TO THE HARDWARE WITH THE NAME
  return ContentService.createTextOutput(studentName);
}
