class RoomProfilesController < ApplicationController
  
  layout "mylistings"
  
  before_filter :login_required

  # GET /room_profiles
  # GET /room_profiles.xml
  def index
    @room_profiles = RoomProfile.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @room_profiles }
    end
  end
  
  def when
    render :text => "This means blah blah blah"
  end
    
  # GET /room_profiles/1
  # GET /room_profiles/1.xml
  def show
    @room_profile = RoomProfile.find(params[:id])
    @room = Room.find(@room_profile.room_id)

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @room_profile }
    end
  end

  # GET /room_profiles/new
  # GET /room_profiles/new.xml
  def new
    @room_profile = RoomProfile.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @room_profile }
    end
  end

  # GET /room_profiles/1/edit
  def edit
    @room_profile = RoomProfile.find(params[:id])
  end
  
  # POST /room_profiles
  # POST /room_profiles.xml
  def create
    @room_profile = RoomProfile.new(params[:room_profile])
    
    respond_to do |format|
        
      if @room_profile.save
        
        if @room_profile.google_map == "No"
          flash[:notice] = 'RoomProfile was successfully created.'
          format.html { redirect_to(@room_profile)} #'/marker/map')}
          format.xml  { render :xml => @room_profile, :status => :created, :location => @room_profile }
        else
          format.html { redirect_to(new_marker_path(:room_id => @room_profile.room_id))} #'/marker/map')}
          format.xml  { render :xml => @room_profile, :status => :created, :location => @room_profile }
        end
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @room_profile.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /room_profiles/1
  # PUT /room_profiles/1.xml
  def update
    @room_profile = RoomProfile.find(params[:id])

    respond_to do |format|
      if @room_profile.update_attributes(params[:room_profile])
        flash[:notice] = 'RoomProfile was successfully updated.'
        format.html { redirect_to(@room_profile) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @room_profile.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /room_profiles/1
  # DELETE /room_profiles/1.xml
  def destroy
    @room_profile = RoomProfile.find(params[:id])
    @room_profile.destroy

    respond_to do |format|
      format.html { redirect_to(room_profiles_url) }
      format.xml  { head :ok }
    end
  end
end
