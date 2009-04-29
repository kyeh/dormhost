class MarkersController < ApplicationController
  
  def show
    @room = Room.find(params[:id])
    puts 
    render :action=> 'index', :layout => false
  end
  
  def created
    @room_profile = RoomProfile.find(params[:marker][:id])
    flash[:notice] = 'RoomProfile was successfully created.'
    redirect_to :controller => :room_profiles , :action => :show, :id=> params[:marker][:id] 
  end
  
  def create   
    @marker = Marker.new(params[:m])
    if @marker.save
      res={:success=>true,:content=>"<div><strong>Details: </strong>#{@marker.details}</div>"}
    else
      res={:success=>false,:content=>"Could not save the marker"}
    end
    render :text=>res.to_json
  end
  
  def get_marker
   render :text=>(Marker.find(params[:m][:id])).to_json
  end
  
  def update
    @marker = Marker.find(params[:m][:room_id])
    if @marker.update_attributes(params[:m])
      res={:success=>true,:content=>"<div><strong>Details: </strong>#{@marker.details}</div>"}
    else
      res={:success=>false,:content=>"Could not save the marker"}
    end
    render :text=>res.to_json
    
  end
  
  def new
    @room = Room.find(params[:room_id])
  end
  
  def list
    render :text=>(Marker.find :all).to_json
  end
end
