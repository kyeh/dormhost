class MarkerController < ApplicationController
  def create  
    marker = Marker.new(params[:m])
    #puts params[:m][:details]
    if marker.save
      res={:success=>true,:content=>"<div><strong>Details: </strong>#{marker.details}</div><div><strong>Price: </strong>#{marker.price}</div>"}
    else
      res={:success=>false,:content=>"Could not save the marker"}
    end
    render :text=>res.to_json
  end
  
  def list
    render :text=>(Marker.find :all).to_json
  end
  
end
