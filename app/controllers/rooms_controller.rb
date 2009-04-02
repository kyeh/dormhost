class RoomsController < ApplicationController
  layout "mylistings"
  
  before_filter :login_required, :except => :all
  
  require 'time'
  
  def search
      @search_params = params[:room][:q].to_s
      
      if (@search_params == "")
        @xap_results = Room.find(:all)
      else
        # Search by location
        if (params[:filter] == "Location")
          @xap_search = ActsAsXapian::Search.new([Room], "(city:" + @search_params + ") OR (state:" + @search_params + ") OR (zip:" + @search_params + ")", { :limit => 100 })
          @xap_results = @xap_search.results.collect { |r| r[:model] }
        
        
        #Search by college/university
        elsif (params[:filter] == "School")
          @xap_results = Array.new
          
          # Find all colleges with names that match the serach params
          @search1 = ActsAsXapian::Search.new([College], @search_params, { :limit => 100 })
          @results1 = @search1.results.collect { |r| r[:model] }
          
          # Find all rooms that match the colleges found
          for result1 in @results1
            @search2 = ActsAsXapian::Search.new([Room], "college:" + result1.id.to_s, { :limit => 100 })
            @results2 = @search2.results.collect { |r| r[:model] }
            
            # Add all found rooms to temp array
            for result2 in @results2
              @xap_results.push(result2)
            end
          end
        
      
        # Search by host
        elsif (params[:filter] == "Host")
          @host_results = Array.new
          @xap_results = Array.new
        
          # Find all users with names that match the search params
          @search1 = ActsAsXapian::Search.new([User], @search_params, { :limit => 100 })
          @results1 = @search1.results.collect { |r| r[:model] }
        
          # Find all the hosts that match the users found
          for result1 in @results1
            @search2 = ActsAsXapian::Search.new([Host], result1.id.to_s, { :limit => 100 })
            @results2 = @search2.results.collect { |r| r[:model] }
          
            # Add all found hosts to temp array
            for result2 in @results2
              @host_results.push(result2)
            end          
          end
        
          # Find all the rooms that match the hosts found
          for host in @host_results
            @temp = Room.find(:all, :conditions => ['host_id = ?', host.id])
            
            for tmp in @temp
              @xap_results.push(tmp)
            end
          end
        
        
        # Search by interests
        elsif (params[:filter] == "Interests")
          @host_results = Array.new
          @xap_results = Array.new
        
          # Find all profiles with interests that match the search params
          @search1 = ActsAsXapian::Search.new([Profile], @search_params, { :limit => 100 })
          @results1 = @search1.results.collect { |r| r[:model] }
        
          # Find all the hosts that match the profiles found
          for result1 in @results1
            @search2 = ActsAsXapian::Search.new([Host], result1.user_id.to_s, { :limit => 100 })
            @results2 = @search2.results.collect { |r| r[:model] }
          
            # Add all found hosts to temp array
            for result2 in @results2
              @host_results.push(result2)
            end
          end
        
          # Find all the rooms that match the hosts found
          for host in @host_results
            @temp = Room.find(:all, :conditions => ['host_id = ?', host.id])
            
            for tmp in @temp
              @xap_results.push(tmp)
            end
          end
        
        # Find rooms that match all conditions
        else
          @host_results = Array.new
          @xap_results = Array.new
        
          # Find all profiles with interests that match the search params
          @search1 = ActsAsXapian::Search.new([Profile], @search_params, { :limit => 100 })
          @results1 = @search1.results.collect { |r| r[:model] }
        
          # Find all users with names that match the search params
          @search2 = ActsAsXapian::Search.new([User], @search_params, { :limit => 100 })
          @results2 = @search2.results.collect { |r| r[:model] }
        
          # Find all the hosts that match the profiles found
          for result1 in @results1
            @search3 = ActsAsXapian::Search.new([Host], result1.user_id.to_s, { :limit => 100 })
            @results3 = @search3.results.collect { |r| r[:model] }
          
            # Add all found hosts to temp array
            for result3 in @results3
              @host_results.push(result3)
            end
          end
        
          # Find all the hosts that match the users found
          for result2 in @results2
            @search4 = ActsAsXapian::Search.new([Host], result2.id.to_s, { :limit => 100 })
            @results4 = @search4.results.collect { |r| r[:model] }
          
            # Add all found hosts to temp array
            for result4 in @results4
              @host_results.push(result4)
            end
          end
        
          # Find all the rooms that match the hosts found
          for host in @host_results
            @temp = Room.find(:all, :conditions => ['host_id = ?', host.id])
            
            for tmp in @temp
              @xap_results.push(tmp)
            end
          end
          
          # Find all colleges that match search params
          @search5 = ActsAsXapian::Search.new([College], @search_params, { :limit => 100 })
          @results5 = @search5.results.collect { |r| r[:model] }
          
          # Find all rooms that match the colleges found
          for result5 in @results5
            @search6 = ActsAsXapian::Search.new([Room], "college:" + result5.id.to_s, { :limit => 100 })
            @results6 = @search6.results.collect { |r| r[:model] }
            
            # Add all found rooms to temp array
            for result6 in @results6
              @xap_results.push(result6)
            end
          end
          
          # Find all rooms that contain search params
          @room_search = ActsAsXapian::Search.new([Room], @search_params, { :limit => 100 })
          @room_results = @room_search.results.collect { |r| r[:model] }
        
          # Add all room results to final output array
          for room in @room_results
            @xap_results.push(room)
          end
        end
      end

      respond_to do |format|
        format.html 
        format.xml  { render :xml => @xap_results, :layout => "planatrip"}        
      end
    end
  
  
  # GET /rooms
  # GET /rooms.xml
  def index
  @user = get_user
  @host = Host.find(:first, :conditions => ['user_id = ?', @user.id])
  @rooms = Room.find(:all, :conditions => ['host_id = ?', @host.id])
  
  @reservations = Array.new
  for room in @rooms 
    reservations = Transaction.find(:all, :conditions => ['approved = ? and room_id = ?', nil, room.id])
    if !reservations.empty?
      for reservation in reservations
        @reservations.push(reservation)
      end
    end
  end
	

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
    
    @modifications = Array.new
    for transaction in @transactions
      modification = Modification.find(:first, :conditions => ['transaction_id = ?', transaction.id])
      @modifications.push(modification)
    end
     
  
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
    @reserved = Transaction.find(:first, :conditions => ['room_id = ? and renter_id = ?', @room.id, @renter.id])
    @host = Host.find(:first, :conditions => ['user_id = ?', @user.id])
     
	
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
    
    @past_transactions = Transaction.find(:all, :conditions => ['renter_id = ? AND approved = ?', @renter.id, true])
    @past_trips = Array.new
    for past_transaction in @past_transactions
      modification = Modification.find(:first, :conditions => ['transaction_id = ?', past_transaction.id], :order => 'id DESC')
      puts "******************************************"
      #puts past_transaction.id
      #puts modification.check_out
      #puts modification.to_yaml
      puts "-----------------------------------------"
      if(modification.check_out.to_time < Time.now)
        @past_trips.push(modification)
      end
    end
    puts @past_trips.size.to_s + " = size ********************"
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
