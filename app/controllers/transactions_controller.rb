class TransactionsController < ApplicationController
  # GET /transactions
  # GET /transactions.xml
  
  layout 'mylistings'
  
  def index
    @transactions = Transaction.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @transactions }
    end
  end

  # GET /transactions/1
  # GET /transactions/1.xml
  def show
    @transaction = Transaction.find(params[:id])
    @renter = @transaction.renter
    @user = User.find(@renter.user_id)
    @modifications = Modification.find(:all, :conditions => ['transaction_id = ?', @transaction.id])
    
    @last_modification = @modifications.reverse[0]

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @transaction }
    end
  end

  # GET /transactions/new
  # GET /transactions/new.xml
  def new
    @transaction = Transaction.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @transaction }
    end
  end

  # GET /transactions/1/edit
  def edit
    @transaction = Transaction.find(params[:id])
  end

  # POST /transactions
  # POST /transactions.xml
  def create
    #@transaction = Transaction.new(params[:transaction])
    
    @transaction = Transaction.new(params[:transaction])
    #@transaction.save
    
    @modification = Modification.new(params[:modification])
    #@modification.transaction_id = @transaction.id
    #puts @modification.to_yaml

    respond_to do |format|
      
      @transaction.request_rooms
            
      if @transaction.save
        
          @modification.transaction_id = @transaction.id
          if @modification.save
            flash[:notice] = 'Transaction was successfully created.'
            format.html { redirect_to('/transactions/manage_transaction/' + @transaction.id.to_s) }
            format.xml  { render :xml => @transaction, :status => :created, :location => @transaction }
          end
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @transaction.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /transactions/1
  # PUT /transactions/1.xml
  def update
    @transaction = Transaction.find(params[:id])
    
    @modification = Modification.find(params[:modification][:id])
    
    #puts "-------------------------------------------"
    #puts params[:modification].to_i
    #puts "-------------------------------------------"
        
    
    respond_to do |format|
      @transaction.approve_reservation
      if @transaction.update_attributes(params[:transaction])
        @modification.update_attribute(:approved,@transaction.approved)
        flash[:notice] = 'Transaction was successfully updated.'
        format.html { redirect_to(@transaction) }
        format.xml  { head :ok }
        
      else
        
        format.html { render :action => "edit" }
        format.xml  { render :xml => @transaction.errors, :status => :unprocessable_entity }
        
      end
    end
  end
  
  def manage_transaction
    
    @transaction = Transaction.find(params[:id])
    @renter = @transaction.renter
    @user = User.find(@renter.user_id)
    @modifications = Modification.find(:all, :conditions => ['transaction_id = ?', @transaction.id])
    @room_review = RoomReview.find(:first, :conditions => ['transaction_id = ?', @transaction.id])
    @user_review = UserReview.find(:first, :conditions => ['transaction_id = ?', @transaction.id])
    
    @last_modification = @modifications[0] 
    
  end

  # DELETE /transactions/1
  # DELETE /transactions/1.xml
  def destroy
    @transaction = Transaction.find(params[:id])
    @transaction.destroy

    respond_to do |format|
      format.html { redirect_to(transactions_url) }
      format.xml  { head :ok }
    end
  end
end
