class FavoritesController < ApplicationController
  
  # GET /favorites
  # GET /favorites.xml
  def index
    @favorites = Favorites.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @favorites }
    end
  end

  # GET /favorites/1
  # GET /favorites/1.xml
  def show
    @favorites = Favorites.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @favorites }
    end
  end

  # GET /favorites/new
  # GET /favorites/new.xml
  def new
    @favorite = Favorite.new
    @room = Room.find(session[:room_id])
    @user = get_user

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @favorite }
    end
  end

  # GET /favorites/1/edit
  def edit
    @favorites = Favorites.find(params[:id])
  end

  # POST /favorites
  # POST /favorites.xml
  def create
    puts params[:favorite][:room_id] 
    puts params[:favorite][:user_id] 
    puts params[:favorite][:user_id]
    
    @room = Room.find(:first, :conditions => ['id = ?', params[:favorite][:room_id]])
    
    puts "-----"
    puts @room.id.to_s + "----------"
    
    @user = params[:favorite][:user_id]
    @favorite = Favorite.new(params[:favorite])
    @favorite.attributes = params[:favorite]
#    @favorite.room_id = @room.id
#    @favorite.user_id = params[:favorite][:user_id]
    puts @room.id.to_s + "HERE"

    respond_to do |format|
      puts @favorite.room_id
      puts @favorite.user_id
      if @favorite.save
        flash[:notice] = 'Room was successfully added to your favorites!'
        format.html { redirect_to(@room) }
        format.xml  { render :xml => @favorite, :status => :created, :location => @favorite }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @favorite.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /favorites/1
  # PUT /favorites/1.xml
  def update
    @favorites = Favorites.find(params[:id])

    respond_to do |format|
      if @favorites.update_attributes(params[:favorites])
        flash[:notice] = 'Favorites was successfully updated.'
        format.html { redirect_to(@favorites) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @favorites.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /favorites/1
  # DELETE /favorites/1.xml
  def destroy
    @room = Room.find(:first, :conditions => ['id = ?', session[:room_id]])
    @favorites = Favorite.find(params[:id])
    @favorites.destroy

    respond_to do |format|
      format.html { redirect_to(@room) }
      format.xml  { head :ok }
    end
  end
end
