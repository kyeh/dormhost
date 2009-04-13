class RoomProfile < ActiveRecord::Base
  has_one :room
  belongs_to :level
  has_many :room_images
  has_many :room_profile_days
  
  has_attached_file :photo
  
  before_validation :clear_photo
  validates_presence_of :bed_type, :max_capacity, :pets, :food, :roommates, :cost, :notes, :message => "must be filled in"
  validates_numericality_of :max_capacity, :roommates, :cost, :message => "must be a number"
  
  def is_updatable_by(user)
    user.is_admin?
  end
  
  def self.is_admin?
    self.is_admin
  end
  
  def self.smoker?
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
  


  # Later in the model
  def clear_photo
    self.photo = nil if delete_photo? && !photo.dirty?
  end
  
end
