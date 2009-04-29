class MarkersController < ApplicationController
  def show
    redirect_to 'map'
  end
  
  def create   
    @marker = Marker.new(params[:m])
    @marker.room_id = 4
    if @marker.save
      res={:success=>true,:content=>"<div><strong>Details: </strong>#{@marker.details}</div>"}
    else
      res={:success=>false,:content=>"Could not save the marker"}
    end
    render :text=>res.to_json
  end
  
  
  def list
    render :text=>(Marker.find :all).to_json
  end
end
