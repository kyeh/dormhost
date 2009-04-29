// Place your application-specific JavaScript functions and classes here
// This file is automatically included by javascript_include_tag :defaults
var map;
var centerLatitude =  10.4411801;
var centerLongitude = -170.9428294;
var startZoom = 15;
var change = false;
var mapListener;
var marker

var room_id = getParameters('room_id');
var geocoder = new GClientGeocoder(); 

 function findAddress(address) {  
    geocoder.getLatLng(  
        address,  
        function(pt) {  
        if (!pt) {  
            var location = new GLatLng(centerLatitude,centerLongitude);
		    map.setCenter(location,startZoom,G_HYBRID_MAP);
			
        } else {  
			map.setCenter(pt, startZoom,G_HYBRID_MAP);  
			
    	}  
    }  
	);  
}
  
function getParameters(name)
{
  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
  var regexS = "[\\?&]"+name+"=([^&#]*)";
  var regex = new RegExp( regexS );
  var results = regex.exec( window.location.href );
  if( results == null )
    return "";
  else
    return results[1];
}

function createMarker(){
	var lng = document.getElementById("longitude").value;
	var lat = document.getElementById("latitude").value;
	
	var getVars = "?m[details]=" + document.getElementById("details").value +
	"&m[lng]=" +
	lng +
	"&m[lat]=" +
	lat +
	"&m[room_id]=" +
	room_id;
	
	var request = GXmlHttp.create();
	
	//call the create action back on the server
	if (change == false) {
		request.open('GET', 'http://localhost:3000/markers/create' + getVars, true);
		
	}
	else {
		request.open('GET', 'http://localhost:3000/markers/update' + getVars, true);	
	}
	request.onreadystatechange = function(){
		if (request.readyState == 4) {
			//the request is complete.
			
			var success = false;
			var content = 'Error contacting web service';
			try {
				//parse the result to JSON(simply by eval-ing it)
				res=eval("(" + request.responseText + ")");
				content=res.content;
				success=res.success;
			} 
			catch (e) {
				success = false;
			}
			
			//check to see if it was an error or success
			if (!success) {
				alert(content);
			}
			else {
				//create a new marker and add its info window
				var latlng = new GLatLng(parseFloat(lat), parseFloat(lng));
				marker = addMarkerToMap(latlng, content);
				map.addOverlay(marker);
				map.closeInfoWindow();

				//GEvent.removeListener(mapListener);
			
				//map.setCenter(lat,lng);
			}
		}
	}
	request.send(null);
	return false;
}

function undoMap() {
	change = true;
	map.removeOverlay(marker)
	mapListener = GEvent.addListener(map,'click', function(overlay,latlng){
			//create an HTML DOM form element.
			var inputForm = document.createElement("form");
			inputForm.setAttribute("action", "");
			inputForm.onsubmit = function(){
				createMarker();
				return false;
			};
			
			//retrieve the longitude and latitude of the click point
			var lng = latlng.lng();
			var lat = latlng.lat();
			
			inputForm.innerHTML = '<fieldset style ="width:200px;">' +
			'<legend>Mark Your Room</legend>' +
			'<label for="details">Details</label>' +
			'<input type="text" id="details" name="m[details]" style="width:100%;" />' +
			'<input type="submit" value="Save Location" />' +
			'<input type="hidden" id="longitude" name="m[lng]" value="' +
			lng +
			'"/>' +
			'<input type="hidden" id="latitude" name="m[lat]" value="' +
			lat +
			'"/>' +
			'<input type="hidden" id="room_id" name="m[room_id]" value="' +
			room_id +
			'"/>'
			'</fieldset>';
			
			//open the infoWindow for the form when click
			map.openInfoWindow(latlng, inputForm);
		});
		
}

//html is res.content in this case
function addMarkerToMap(latlng,html){
	var marker1 = new GMarker(latlng);
	GEvent.addListener(marker1, 'click', function(){
		var markerHTML = html;
		marker1.openInfoWindowHtml(markerHTML);
	});
	return marker1;
}

function listMarkers() {
	
	var request = GXmlHttp.create();
	
	//tell the request where to retrieve data from.
	request.open('GET', 'list', true);
	
	//request object has state when doing a get
	//tell the request what to do when the state changes.
	request.onreadystatechange = function(){
		if(request.readyState == 4){
			//parse the result to JSON, by eval-ing it.
			//the response is an array of markers
			markers=eval( "(" + request.responseText + ")");
			
			for(var i = 0; i< markers.length ; i++){
				
				var marker=markers[i].marker;
				var lat=marker.lat;
				var lng=marker.lng;
				
				//check for lat and lng so MSIE does not error
				//on parseFloat of a null value
				if (lat && lng){
					var latlng = new GLatLng(parseFloat(lat),parseFloat(lng));
					
					var html = '<div><b>Details:</b> '
						+ marker.details
						+ '</div>';
						
					var marker = addMarkerToMap(latlng,html);
					map.addOverlay(marker);
					
				}//end of if lat and lng
			}//end of for loop
		}//if
	}//function
	
	request.send(null);
}


function init()
{
	if (GBrowserIsCompatible()){
		map = new GMap2(document.getElementById("map"));
		//listMarkers();
		//
		map.addControl(new GSmallMapControl());
		map.addControl(new GMapTypeControl());
		
		findAddress(full_address);
		
		//var location = new GLatLng(centerLatitude,centerLongitude);
		//map.setCenter(location,startZoom);
		
		mapListener = GEvent.addListener(map,'click', function(overlay,latlng){
			//create an HTML DOM form element.
			var inputForm = document.createElement("form");
			inputForm.setAttribute("action", "");
			inputForm.onsubmit = function(){
				createMarker();
				return false;
			};
			
			//retrieve the longitude and latitude of the click point
			var lng = latlng.lng();
			var lat = latlng.lat();
			
			inputForm.innerHTML = '<fieldset style ="width:200px;">' +
			'<legend>Mark Your Room</legend>' +
			'<label for="details">Details</label>' +
			'<input type="text" id="details" name="m[details]" style="width:100%;" />' +
			'<input type="submit" value="Save Location" />' +
			'<input type="hidden" id="longitude" name="m[lng]" value="' +
			lng +
			'"/>' +
			'<input type="hidden" id="latitude" name="m[lat]" value="' +
			lat +
			'"/>' +
			'<input type="hidden" id="room_id" name="m[room_id]" value="' +
			room_id +
			'"/>'
			'</fieldset>';
			
			//open the infoWindow for the form when click
			map.openInfoWindow(latlng, inputForm);
		});
	}
}

  

window.onload =init;
window.onunload = GUnload;