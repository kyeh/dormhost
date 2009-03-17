# Filters added to this controller apply to all controllers in the application.
# Likewise, all the methods added will be available for all controllers.

class ApplicationController < ActionController::Base
  helper :all # include all helpers, all the time
  include AuthenticatedSystem
  

  # See ActionController::RequestForgeryProtection for details
  # Uncomment the :secret if you're not using the cookie session store
  protect_from_forgery # :secret => 'd65f942a7269eb06c84d2c383f21b6f8'
  
  # See ActionController::Base for details 
  # Uncomment this to filter the contents of submitted sensitive data parameters
  # from your application log (in this case, all fields with names like "password"). 
  # filter_parameter_logging :password
  
  def search
    @user = get_user
    @xap_search = ActsAsXapian::Search.new([Room], params[:q].to_s, { :limit => 100 })
    @xap_results = @xap_search.results.collect { |r| r[:model] }
    
    respond_to do |format|
      format.html { redirect_to(rooms_url) }
      format.xml  { render :xml => @xap_results }
    end
  end
  
  
  def get_user
    @user = User.find(session[:user_id]) unless session[:user_id].nil?
    @user = @user ||= User.new
  end
end
