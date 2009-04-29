/**
 * @author yaoween
 */

var map;
var centerLatitude =  10.4411801;
var centerLongitude = -170.9428294;
var startZoom = 15;

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

//html is res.content in this case
function addMarkerToMap(latlng,html){
	var marker2 = new GMarker(latlng);
	
	GEvent.addListener(marker2, 'click', function(){
		var markerHTML = html;
		marker2.openInfoWindowHtml(markerHTML);
	});
	return marker2;
}

function listMarkers() {
	
	var request = GXmlHttp.create();
	
	//tell the request where to retrieve data from.
	request.open('GET', 'http://localhost:3000/markers/list', true);
	
	//request object has state when doing a get
	//tell the request what to do when the state changes.
	request.onreadystatechange = function(){
		if(request.readyState == 4){
			//parse the result to JSON, by eval-ing it.
			//the response is an array of markers
			markers=eval( "(" + request.responseText + ")");
			for (var i = 0; i < markers.length; i++) {
			
				var marker = markers[i].marker;
				
				if (marker.room_id == id) {
				
				var lat = marker.lat;
				var lng = marker.lng;
				
				//check for lat and lng so MSIE does not error
				//on parseFloat of a null value
				if (lat && lng) {
					var latlng = new GLatLng(parseFloat(lat), parseFloat(lng));
					map.setCenter(latlng,startZoom);
					var html = '<div><b>Details:</b> ' +
					marker.details +
					'</div>';
					
					var marker1 = addMarkerToMap(latlng, html);
					map.addOverlay(marker1);
					
				}//end of if lat and lng
			}//end of if
			}//end of for
		}//if
	}//function
	
	request.send(null);
}

 function init()
{
	if (GBrowserIsCompatible()){
		map = new GMap2(document.getElementById("map"));
		listMarkers();
		//
		map.addControl(new GSmallMapControl());
		//map.addControl(new GMapTypeControl());
		
		
		//var location = new GLatLng(centerLatitude,centerLongitude);
		//map.setCenter(location,startZoom);
				
	}
}

window.onload =init;
window.onunload = GUnload;