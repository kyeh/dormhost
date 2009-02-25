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
    @favorites = Favorites.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @favorites }
    end
  end

  # GET /favorites/1/edit
  def edit
    @favorites = Favorites.find(params[:id])
  end

  # POST /favorites
  # POST /favorites.xml
  def create
    @favorites = Favorites.new(params[:favorites])

    respond_to do |format|
      if @favorites.save
        flash[:notice] = 'Favorites was successfully created.'
        format.html { redirect_to(@favorites) }
        format.xml  { render :xml => @favorites, :status => :created, :location => @favorites }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @favorites.errors, :status => :unprocessable_entity }
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
    @favorites = Favorites.find(params[:id])
    @favorites.destroy

    respond_to do |format|
      format.html { redirect_to(favorites_url) }
      format.xml  { head :ok }
    end
  end
end
