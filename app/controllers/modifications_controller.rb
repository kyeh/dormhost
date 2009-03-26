class ModificationsController < ApplicationController
  
  layout "mytrips"
  
  # GET /modifications
  # GET /modifications.xml
  def index
    @modifications = Modification.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @modifications }
    end
  end

  # GET /modifications/1
  # GET /modifications/1.xml
  def show
    @modification = Modification.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @modification }
    end
  end

  # GET /modifications/new
  # GET /modifications/new.xml
  def new
    @modification = Modification.new
    #@transaction = Transaction.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @modification }
    end
  end

  # GET /modifications/1/edit
  def edit
    @modification = Modification.find(params[:id])
  end

  # POST /modifications
  # POST /modifications.xml
  def create
    @transaction = Transaction.new(params[:transaction])
    @transaction.save
    
    @modification = Modification.new(params[:modification])
    @modification.transaction_id = @transaction.id
    #puts @modification.to_yaml
 

    respond_to do |format|
      if @modification.save
        flash[:notice] = 'Modification was successfully created.'
        format.html { redirect_to(@modification) }
        format.xml  { render :xml => @modification, :status => :created, :location => @modification }
      else
        flash[:notice] = 'NOT CREATED. '
        format.html { render :action => "new" }
        format.xml  { render :xml => @modification.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /modifications/1
  # PUT /modifications/1.xml
  def update
    @modification = Modification.find(params[:id])

    respond_to do |format|
      if @modification.update_attributes(params[:modification])
        flash[:notice] = 'Modification was successfully updated.'
        format.html { redirect_to(@modification) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @modification.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /modifications/1
  # DELETE /modifications/1.xml
  def destroy
    @modification = Modification.find(params[:id])
    @modification.destroy

    respond_to do |format|
      format.html { redirect_to(modifications_url) }
      format.xml  { head :ok }
    end
  end
  
  def reservations
    
  end
  
  def requests
    
  end
end
