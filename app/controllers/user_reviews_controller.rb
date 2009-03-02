class UserReviewsController < ApplicationController
  # GET /user_reviews
  # GET /user_reviews.xml
  def index
    @user_reviews = UserReview.find(:all)

    respond_to do |format|
      format.html # index.html.erb
      format.xml  { render :xml => @user_reviews }
    end
  end

  # GET /user_reviews/1
  # GET /user_reviews/1.xml
  def show
    @user_review = UserReview.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @user_review }
    end
  end

  # GET /user_reviews/new
  # GET /user_reviews/new.xml
  def new
    @user_review = UserReview.new

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @user_review }
    end
  end

  # GET /user_reviews/1/edit
  def edit
    @user_review = UserReview.find(params[:id])
  end

  # POST /user_reviews
  # POST /user_reviews.xml
  def create
    @user_review = UserReview.new(params[:user_review])

    respond_to do |format|
      if @user_review.save
        flash[:notice] = 'UserReview was successfully created.'
        format.html { redirect_to(@user_review) }
        format.xml  { render :xml => @user_review, :status => :created, :location => @user_review }
      else
        format.html { render :action => "new" }
        format.xml  { render :xml => @user_review.errors, :status => :unprocessable_entity }
      end
    end
  end

  # PUT /user_reviews/1
  # PUT /user_reviews/1.xml
  def update
    @user_review = UserReview.find(params[:id])

    respond_to do |format|
      if @user_review.update_attributes(params[:user_review])
        flash[:notice] = 'UserReview was successfully updated.'
        format.html { redirect_to(@user_review) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @user_review.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /user_reviews/1
  # DELETE /user_reviews/1.xml
  def destroy
    @user_review = UserReview.find(params[:id])
    @user_review.destroy

    respond_to do |format|
      format.html { redirect_to(user_reviews_url) }
      format.xml  { head :ok }
    end
  end
end