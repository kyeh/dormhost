class Profile < ActiveRecord::Base
  has_many :user_reviews
  belongs_to :college
  has_one :user
  
  
  named_scope :for_user, lambda { |user_id| { :conditions => ["user_id = ?", session[:user_id]] }}
  
  attr_accessible :user_id, :college_id, :birthdate, :grad_year, :phone, :living_style, :bedtime, :interests, :smoker
  
  def is_updatable_by(user)
    user.is_admin?
  end
  
  def self.is_admin?
    self.is_admin
  end
  
  def self.smoker?
    self.smoker
  end
  
  
end
