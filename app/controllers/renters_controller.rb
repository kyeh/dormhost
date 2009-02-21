class RentersController < ApplicationController
  # GET /renters
  # GET /renters.xml
  def index
    @renters = Renter.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @renters }
    end
  end

  # GET /renters/1
  # GET /renters/1.xml
  def show
    @renter = Renter.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @renter }
    end
  end

  # GET /renters/new
  # GET /renters/new.xml
  def new
    @renter = Renter.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @renter }
    end
  end

  # GET /renters/1/edit
  def edit
    @renter = Renter.find(params[:id])
  end

  # POST /renters
  # POST /renters.xml
  def create
    @renter = Renter.new(params[:renter])

    respond_to do |format|
      if @renter.save
        flash[:notice] = 'Renter was successfully created.'
        format.html { redirect_to(@renter) }
        format.xml  { render :xml => @renter, :status => :created, :location => @renter }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @renter.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /renters/1
  # PUT /renters/1.xml
  def update
    @renter = Renter.find(params[:id])

    respond_to do |format|
      if @renter.update_attributes(params[:renter])
        flash[:notice] = 'Renter was successfully updated.'
        format.html { redirect_to(@renter) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @renter.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /renters/1
  # DELETE /renters/1.xml
  def destroy
    @renter = Renter.find(params[:id])
    @renter.destroy

    respond_to do |format|
      format.html { redirect_to(renters_url) }
      format.xml  { head :ok }
    end
  end
end
