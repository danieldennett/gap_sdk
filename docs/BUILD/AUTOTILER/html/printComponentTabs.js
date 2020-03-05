var strgURL =   location.pathname;                      // path of current component

// constuctor for the array of objects
function tabElement(id, folderName, tabTxt )  {
	this.id = id;                                       // elementID as needed in html; 
	this.folderName = folderName;                       // folder name of the component 
    this.tabTxt = tabTxt;                               // Text displayed as menu on the web
	this.currentListItem = '<li id="' + this.id + '" class="current"> <a href="../..' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>';
	this.listItem = '<li id="' + this.id + '"> <a href="../..' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>';
};

// array of objects
var arr = [];

// fill array - patched by makefile
arr.push(new tabElement("HOME", "/HOME/html/", "HOME"));
arr.push(new tabElement("PULP-OS", "/PULP-OS/html/", "PULP-OS"));
arr.push(new tabElement("MBED-OS", "/MBED-OS/html/", "MBED-OS"));
arr.push(new tabElement("FREERTOS", "/FREERTOS/html/", "FREERTOS"));
arr.push(new tabElement("AUTOTILER", "/AUTOTILER/html/", "AUTOTILER"));
arr.push(new tabElement("APPLICATIONS", "/APPLICATIONS/html/", "APPLICATIONS"));
arr.push(new tabElement("GVSOC", "/GVSOC/html/", "GVSOC"));
arr.push(new tabElement("BENCHMARKS", "/BENCHMARKS/html/", "BENCHMARKS"));
arr.push(new tabElement("PMSIS_API", "/PMSIS_API/html/", "PMSIS_API"));
arr.push(new tabElement("PMSIS_BSP", "/PMSIS_BSP/html/", "PMSIS_BSP"));
 
// write tabs
// called from the header file.
function writeComponentTabs()  {
  for ( var i=0; i < arr.length; i++ ) {
    if (strgURL.search(arr[i].folderName) > 0) {                    // if this is the current folder
      document.write(arr[i].currentListItem);                       // then print and hightlight the tab
    } else {                                                      
      document.write(arr[i].listItem);                              // else, print the tab
    }                                                             
  }
};
