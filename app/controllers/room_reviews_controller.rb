class RoomReviewsController < ApplicationController
  # GET /room_reviews
  # GET /room_reviews.xml
  
  layout 'mytrips'
  def index
    @room_reviews = RoomReview.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @room_reviews }
    end
  end

  # GET /room_reviews/1
  # GET /room_reviews/1.xml
  def show
    @room_review = RoomReview.find(params[:id])
    @transaction = Transaction.find(@room_review.transaction_id)
    @room = Room.find(@transaction.room_id)

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @room_review }
    end
  end

  # GET /room_reviews/new
  # GET /room_reviews/new.xml
  def new
    @room_review = RoomReview.new
    @transaction = Transaction.find(params[:transaction_id])
    @room = Room.find(@transaction.room_id)
    #@room = Room.find(:first, :conditions => ['id = ?', ])

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @room_review }
    end
  end

  # GET /room_reviews/1/edit
  def edit
    @room_review = RoomReview.find(params[:id])
  end

  # POST /room_reviews
  # POST /room_reviews.xml
  def create
    @room_review = RoomReview.new(params[:room_review])

    respond_to do |format|
      if @room_review.save
        flash[:notice] = 'RoomReview was successfully created.'
        format.html { redirect_to(@room_review) }
        format.xml  { render :xml => @room_review, :status => :created, :location => @room_review }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @room_review.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /room_reviews/1
  # PUT /room_reviews/1.xml
  def update
    @room_review = RoomReview.find(params[:id])

    respond_to do |format|
      if @room_review.update_attributes(params[:room_review])
        flash[:notice] = 'RoomReview was successfully updated.'
        format.html { redirect_to(@room_review) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @room_review.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /room_reviews/1
  # DELETE /room_reviews/1.xml
  def destroy
    @room_review = RoomReview.find(params[:id])
    @room_review.destroy

    respond_to do |format|
      format.html { redirect_to(room_reviews_url) }
      format.xml  { head :ok }
    end
  end
end
