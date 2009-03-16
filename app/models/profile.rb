class Profile < ActiveRecord::Base
  has_many :user_reviews
  belongs_to :college
  has_one :user
 
  named_scope :for_user, lambda { |user_id| { :conditions => ["user_id = ?", user_id] }}
  
  attr_accessible :user_id, :college_id, :birthdate, :grad_year, :phone, :living_style, :bedtime, :interests, :smoker
  
  #check validity of fields
  #NOTE: have not required interests (3/4/09)
  validates_presence_of :college_id
  validates_format_of :phone, :with => /^([\(]{1}[0-9]{3}[\)]{1}[\.| |\-]{0,1}|^[0-9]{3}[\.|\-| ]?)?[0-9]{3}(\.|\-| )?[0-9]{4}$/
  
  def is_updatable_by(user)
    user.is_admin?
  end
  
  def self.is_admin?
    self.is_admin
  end
  
  def self.is_smoker?
    self.smoker
  end
  
  
end
