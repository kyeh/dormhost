class Profile < ActiveRecord::Base
  has_many :user_reviews
  belongs_to :college
  has_one :user
  
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
