class HomeController < ApplicationController
  
  layout "application"
  
  def index
     render :action => :home
     
  end
  
  def show
    render :action => params[:page]
    
  end
end

