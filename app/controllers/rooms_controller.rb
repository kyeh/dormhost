class RoomsController < ApplicationController
  layout "mylistings"
  
  before_filter :login_required, :except => :all
  
  def search
      @search_params = params[:room][:q].to_s

      if (params[:filter] == "Location")
        @xap_search = ActsAsXapian::Search.new([Room], "(city:" + @search_params + ") OR (state:" + @search_params + ") OR (zip:" + @search_params + ")", { :limit => 100 })
      elsif (params[:filter] == "School")
        unless @search_params.nil?
          @spec = College.find(:first, :conditions => ['name = ?', @search_params])
          unless @spec.nil?
            @spec = @spec.id
          end
            @xap_search = ActsAsXapian::Search.new([Room], "college:" + @spec.to_s, { :limit => 100})
        end
      else
        @xap_search = ActsAsXapian::Search.new([Room], @search_params, { :limit => 100 })
      end

      @xap_results = @xap_search.results.collect { |r| r[:model] }

      respond_to do |format|
        format.html 
        format.xml  { render :xml => @xap_results }
      end
    end
  
  
  # GET /rooms
  # GET /rooms.xml
  def index
  @user = get_user
  @host = Host.find(:first, :conditions => ['user_id = ?', @user.id])
  @rooms = Room.find(:all, :conditions => ['host_id = ?', @host.id])
	

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @rooms }
    end
  end


  def show_private
    
    @room = Room.find(params[:id])
    @college= @room.college
    @room_profile = RoomProfile.find(:first, :conditions => ['room_id = ?', params[:id]])

    @room_profile_id = @room_profile.id unless @room_profile.nil?
    
    @transactions = Transaction.find(:all, :conditions => ['room_id = ?', @room.id])
     
  
    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @room }
    end
  end

  # GET /rooms/1
  # GET /rooms/1.xml
  def show
	
    @room = Room.find(params[:id])
    @college= @room.college
    @room_profile = RoomProfile.find(:all, :conditions => ['room_id = ?', params[:id]])
    @user = get_user
    #FIX THIS so that "Request this room" doesn't show if the renter has already requested the room
    @renter = Renter.find(:first, :conditions => ['user_id = ?', @user.id])
    @reserved = Transaction.find(:first, :conditions => ['room_id = ?', @room.id])
     
	
    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @room }
    end
  end
  
  def requested
    @user = get_user
    @renter = Renter.find(:first, :conditions => ['user_id = ?', @user.id])
    @transactions = Transaction.find(:all, :conditions => ['renter_id = ?', @renter.id])
    @rooms = Array.new
    for transaction in @transactions
      room = Room.find(:first, :conditions => ['id = ?', transaction.room_id])
      @rooms.push(room)
    end
  end
  
  def all
    user = get_user
    
    if !logged_in?
      @rooms = Room.find(:all)
    else
      @host = Host.find(:first, :conditions => ['user_id = ?', user.id])
      session[:host_id] = @host.id
      @rooms = Room.find(:all, :conditions => ['host_id != ?', session[:host_id]])
    end
    
    respond_to do |format|
      format.html # all.html.erb
      format.xml  { render :xml => @rooms }
    end
  end

  # GET /rooms/new
  # GET /rooms/new.xml
  def new
    @room = Room.new
    @user = get_user
    @host = Host.find(:first, :conditions => ['user_id = ?', @user.id])
    session[:host_id] = @host.id

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @room }
    end
  end

  # GET /rooms/1/edit
  def edit
    @room = Room.find(params[:id])
  end

  # POST /rooms
  # POST /rooms.xml
  def create
    @room = Room.new(params[:room])

    respond_to do |format|
      if @room.save
        flash[:notice] = 'Room was successfully created.'
        format.html { redirect_to(@room) }
        format.xml  { render :xml => @room, :status => :created, :location => @room }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @room.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /rooms/1
  # PUT /rooms/1.xml
  def update
    @room = Room.find(params[:id])

    respond_to do |format|
      if @room.update_attributes(params[:room])
        flash[:notice] = 'Room was successfully updated.'
        format.html { redirect_to(@room) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @room.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /rooms/1
  # DELETE /rooms/1.xml
  def destroy
    @room = Room.find(params[:id])
    @room.destroy

    respond_to do |format|
      format.html { redirect_to(rooms_url) }
      format.xml  { head :ok }
    end
  end
end
