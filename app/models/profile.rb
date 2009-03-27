class Profile < ActiveRecord::Base
  has_many :user_reviews
  belongs_to :college
  has_one :user
  
  acts_as_xapian  :texts => [:interests]
  
 
  named_scope :for_user, lambda { |user_id| { :conditions => ["user_id = ?", user_id] }}
  
  attr_accessible :user_id, :college_id, :birthdate, :grad_year, :phone, :living_style, :bedtime, :interests, :smoker, :photo
  
  has_attached_file :photo
  
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
  
  #photo stuff
  def delete_photo=(value)
    @delete_photo = !value.to_i.zero?
  end
  
  def delete_photo
    !!@delete_photo
  end
  alias_method :delete_photo?, :delete_photo
  
  before_validation :clear_photo

  # Later in the model
  def clear_photo
    self.photo = nil if delete_photo? && !photo.dirty?
  end

  
end
