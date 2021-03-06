class UsersController < ApplicationController
  # Be sure to include AuthenticationSystem in Application Controller instead
  include AuthenticatedSystem
  
  layout "application"
  # Protect these actions behind an admin login
  # before_filter :admin_required, :only => [:suspend, :unsuspend, :destroy, :purge]
  before_filter :find_user, :only => [:suspend, :unsuspend, :destroy, :purge]
  

  # render new.rhtml
  def new
    @user = User.new
  end
  
  # GET /users/1/edit
  def edit
    @user = User.find(params[:id])
  end
  
  def index
    @user = get_user
    @users = User.paginate :page => params[:page], :order => 'created_at DESC'
  end
 
  def create
    logout_keeping_session!
    @user = User.new(params[:user])
    if verify_recaptcha(@user)
      
      puts "*****************" + @user.terms.to_s
      puts "*****************" + @user.username.to_s
      
      @user.register! if @user && @user.valid? && @user.terms == true
      success = @user && @user.valid? && @user.terms == true
      
      if success && @user.errors.empty?

        #creates a record in the host & renter table
        
        @host = Host.new
        @host.user_id = @user.id
        
        @renter = Renter.new
        @renter.user_id = @user.id
        
        success = @host.save && @renter.save
        if success && @host.errors.empty? && @renter.errors.empty?
          
          render :action =>'create'
        
        else
          flash[:error]  = "We couldn't set up that account, sorry.  Please try again, or contact an admin (link is above)."
          render :action => 'new'
          
        end 
      else
        if @user.terms == false
            @user.errors.add(:terms, "You must approve the terms of service before you can create an account.")
          end
        render :action => 'new'
      end
    
    else
      flash[:error] = "Error verifying with recaptcha please enter the 2 words again and re-enter your password before submission"
      render :action => 'new'
    end
    
  end
  
  # PUT /users/1
  # PUT /users/1.xml
  def update
    @user = User.find(params[:id])

    respond_to do |format|
      if @user.update_attributes(params[:user])
        flash[:notice] = 'User was successfully updated.'
        format.html { redirect_to(users_path) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @user.errors, :status => :unprocessable_entity }
      end
    end
  end

  def activate
    logout_keeping_session!
    user = User.find_by_activation_code(params[:activation_code]) unless params[:activation_code].blank?
    case
    when (!params[:activation_code].blank?) && user && !user.active?
      user.activate!
      flash[:notice] = "Signup complete! Please sign in to continue."
      redirect_to '/login'
    when params[:activation_code].blank?
      flash[:error] = "The activation code was missing.  Please follow the URL from your email."
      redirect_back_or_default('/')
    else 
      flash[:error]  = "We couldn't find a user with that activation code -- check your email? Or maybe you've already activated -- try signing in."
      redirect_back_or_default('/')
    end
  end
  
  def change_password
      return unless request.post?
      if User.authenticate(current_user.username, params[:old_password])
        if ((params[:password] == params[:password_confirmation]) && 
                              !params[:password_confirmation].blank?)
          current_user.password_confirmation = params[:password_confirmation]
          current_user.password = params[:password]

          if current_user.save
            flash.now[:notice] = "Password successfully updated" 
            render :action => 'change_confirmation'
          else
            #puts error_messages_for 'current_user'
            puts "**************************************************"
            flash.now[:notice] = "Password not changed" 
          end

        else
          flash.now[:notice] = "New Password mismatch" 
          @old_password = params[:old_password]
        end
      else
        flash.now[:notice] = "Old password incorrect" 
      end
  end
  
  def forgot_password
      return unless request.post?
      if @user = User.find_by_email(params[:user][:email])
        @user.forgot_password
        @user.save
        render :action => 'confirmation'
      else
        flash.now[:notice] = "Could not find a user with that email address" 
      end
  end
  
  def reset_password
      @user = User.find_by_password_reset_code(params[:id])
      return if @user unless params[:user]
      if ((params[:user][:password] == params[:user][:password_confirmation]) && 
                              !params[:user][:password_confirmation].blank?)
          if (params[:user][:password].length >= 6)
            self.current_user = @user #for the next two lines to work
            current_user.password_confirmation = params[:user][:password_confirmation]
            current_user.password = params[:user][:password]
            @user.reset_password
            current_user.save
            #flash.now[:notice] = "Password reset success."
            logout_keeping_session!
            render :action => 'reset_confirmation'
            
          else
            flash.now[:notice] = "Password reset failure: Password needs to be 6 or more characters. Please try again!"
          end
      else
        flash.now[:notice] = "Password mismatch"
      end  
  end

  def suspend
    @user = User.find(params[:user][:id])
    @user.suspend!
    render :action => 'deactivated'
  end

  def unsuspend
    @user.unsuspend! 
    redirect_to users_path
  end

  def destroy
    @user.delete!
    redirect_to users_path
  end

  def purge
    @user.destroy
    redirect_to users_path
  end
  
  # There's no page here to update or destroy a user.  If you add those, be
  # smart -- make sure you check that the visitor is authorized to do so, that they
  # supply their old password along with a new one to update it, etc.

protected
  def find_user
    @user = User.find(params[:user][:id])
  end

end
