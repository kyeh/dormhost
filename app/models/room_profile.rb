class RoomProfile < ActiveRecord::Base
  has_one :room
  belongs_to :level
  has_many :room_images
  has_many :room_profile_days
  
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
  
end
